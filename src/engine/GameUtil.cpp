#include "GameUtil.h"
#include <SDL3/SDL.h>
#include <picosha2.h>
#include <string>

std::string GameUtil::CalcFileSha256(const std::string &path) {
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open()) {
    return "";
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  std::string data = ss.str();

  std::string outHex = picosha2::hash256_hex_string(data);
  return outHex;
}
std::string GameUtil::ConvertPath(const std::filesystem::path &p) {
#ifdef _WIN32
  std::filesystem::path newPath = p;
  return newPath.make_preferred().string();
#else
  return p.string();
#endif
}

#ifdef _WIN32
std::wstring GameUtil::Utf8ToWide(const std::string &s) {
  if (s.empty())
    return {};
  int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
  std::wstring ws(len - 1, L'\0');
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), len);
  return ws;
}

std::string GameUtil::WideToUtf8(const std::wstring &ws) {
  if (ws.empty())
    return {};
  int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr,
                                nullptr);
  std::string s(len - 1, '\0');
  WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, s.data(), len, nullptr,
                      nullptr);
  return s;
}

std::string GameUtil::GetRegValue(HKEY hKey, const std::string &subKey,
                                  const std::string &value) {

  std::wstring wSubKey = Utf8ToWide(subKey);
  std::wstring wValue = Utf8ToWide(value);

  DWORD size = 0, type = 0;
  if (RegGetValueW(hKey, wSubKey.c_str(), wValue.c_str(),
                   RRF_RT_REG_SZ | RRF_SUBKEY_WOW6432KEY, &type, nullptr,
                   &size) != ERROR_SUCCESS) {
    return {};
  }

  std::wstring wPath(size / sizeof(wchar_t), L'\0');

  if (RegGetValueW(hKey, wSubKey.c_str(), wValue.c_str(),
                   RRF_RT_REG_SZ | RRF_SUBKEY_WOW6432KEY, &type, wPath.data(),
                   &size) != ERROR_SUCCESS) {
    return {};
  }

  if (!wPath.empty() && wPath.back() == L'\0')
    wPath.pop_back();

  return WideToUtf8(wPath);
}

int GameUtil::RunCommand(const std::string &exe,
                         const std::vector<std::string> &args, std::string &out,
                         std::string &err) {
  HANDLE hOutRead, hOutWrite, hErrRead, hErrWrite;
  SECURITY_ATTRIBUTES sa{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};

  // 创建管道
  if (!CreatePipe(&hOutRead, &hOutWrite, &sa, 0))
    return -1;
  if (!CreatePipe(&hErrRead, &hErrWrite, &sa, 0))
    return -1;
  SetHandleInformation(hOutRead, HANDLE_FLAG_INHERIT, 0);
  SetHandleInformation(hErrRead, HANDLE_FLAG_INHERIT, 0);

  STARTUPINFOA si{};
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.hStdOutput = hOutWrite;
  si.hStdError = hErrWrite;
  si.hStdInput = nullptr;

  PROCESS_INFORMATION pi{};

  // 拼接命令行
  std::ostringstream oss;
  oss << '"' << exe << '"';
  for (const auto &arg : args)
    oss << " \"" << arg << '"';
  std::string cmd = oss.str();

  if (!CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, TRUE,
                      CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
    CloseHandle(hOutRead);
    CloseHandle(hOutWrite);
    CloseHandle(hErrRead);
    CloseHandle(hErrWrite);
    return -1;
  }

  CloseHandle(hOutWrite);
  CloseHandle(hErrWrite);

  char buffer[256];
  DWORD n;
  while (ReadFile(hOutRead, buffer, sizeof(buffer), &n, nullptr) && n > 0)
    out.append(buffer, n);
  while (ReadFile(hErrRead, buffer, sizeof(buffer), &n, nullptr) && n > 0)
    err.append(buffer, n);

  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exitCode = 0;
  GetExitCodeProcess(pi.hProcess, &exitCode);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  CloseHandle(hOutRead);
  CloseHandle(hErrRead);
  auto normalize = [](std::string &s) {
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
      if (s[i] == '\r') {
        if (i + 1 < s.size() && s[i + 1] == '\n')
          continue; // 跳过 \r
      }
      out.push_back(s[i]);
    }
    s.swap(out);
  };

  normalize(out);
  normalize(err);
  return static_cast<int>(exitCode);
}
#else
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

int GameUtil::RunCommand(const std::string &exe,
                         const std::vector<std::string> &args, std::string &out,
                         std::string &err) {
  int outPipe[2], errPipe[2];
  if (pipe(outPipe) != 0 || pipe(errPipe) != 0)
    return -1;

  pid_t pid = fork();
  if (pid < 0)
    return -1;
  if (pid == 0) {
    // 子进程
    dup2(outPipe[1], STDOUT_FILENO);
    dup2(errPipe[1], STDERR_FILENO);
    close(outPipe[0]);
    close(outPipe[1]);
    close(errPipe[0]);
    close(errPipe[1]);

    std::vector<char *> argv;
    argv.push_back(const_cast<char *>(exe.c_str()));
    for (const auto &arg : args)
      argv.push_back(const_cast<char *>(arg.c_str()));
    argv.push_back(nullptr);
    execv(exe.c_str(), argv.data());
    _exit(127); // exec失败
  }

  // 父进程
  close(outPipe[1]);
  close(errPipe[1]);
  char buffer[256];
  ssize_t n;
  while ((n = read(outPipe[0], buffer, sizeof(buffer))) > 0)
    out.append(buffer, n);
  while ((n = read(errPipe[0], buffer, sizeof(buffer))) > 0)
    err.append(buffer, n);
  close(outPipe[0]);
  close(errPipe[0]);

  int status = 0;
  waitpid(pid, &status, 0);
  return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

#endif