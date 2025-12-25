#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>

struct Task {
  enum class DownloadState { RUNNING, FINISHED, FAILED };

  int id;
  DownloadState state = DownloadState::RUNNING;
  int progress = 0;
  std::string result;

  int retryCount = 0;
  int maxRetries = 0;

  std::function<void(int, const std::string &)> onSuccess;
  std::function<void(int, const std::string &)> onError;
};

class DownloadManager {
public:
  DownloadManager() = delete;

  static int CreateTask(const std::string &url,
                        std::function<void(int, const std::string &)> onSuccess,
                        std::function<void(int, const std::string &)> onError,
                        int maxRetries = 3);

  static bool GetProgress(int id, int &outProgress);
  static void Cancel(int id);

private:
  static void StartTask(int id, const std::string &url);

  static std::unordered_map<int, Task> tasks;
  static std::mutex mtx;
  static int nextId;
};

#endif
