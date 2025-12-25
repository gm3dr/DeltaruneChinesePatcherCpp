#include "DownloadManager.h"
#include "cpr/error.h"
#include <cpr/cpr.h>
#include <thread>

std::unordered_map<int, Task> DownloadManager::tasks;
std::mutex DownloadManager::mtx;
int DownloadManager::nextId = 1;

int DownloadManager::CreateTask(
    const std::string &url,
    std::function<void(int, const std::string &)> onSuccess,
    std::function<void(int, const std::string &)> onError, int maxRetries) {
  std::lock_guard<std::mutex> lock(mtx);
  int id = nextId++;

  Task task;
  task.id = id;
  task.maxRetries = maxRetries;
  task.onSuccess = std::move(onSuccess);
  task.onError = std::move(onError);

  tasks.emplace(id, std::move(task));

  StartTask(id, url);
  return id;
}

void DownloadManager::StartTask(int id, const std::string &url) {
  std::thread([id, url]() {
    cpr::Response r = cpr::Get(
        cpr::Url(url), cpr::ProgressCallback([id](size_t total, size_t now,
                                                  size_t, size_t, intptr_t) {
          if (total > 0) {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = tasks.find(id);
            if (it != tasks.end()) {
              it->second.progress = int(now * 100 / total);
            }
          }
          return true;
        }));

    std::lock_guard<std::mutex> lock(mtx);
    auto it = tasks.find(id);
    if (it == tasks.end())
      return;

    Task &task = it->second;

    if (r.error.code != cpr::ErrorCode::OK) {
      task.retryCount++;
      if (task.retryCount <= task.maxRetries) {
        StartTask(id, url);
      } else {
        auto cb = task.onError;
        tasks.erase(it);
        if (cb)
          cb(id, "Network error: please check your network, or download the offline patch.");
      }
    } else if (r.status_code != 200) {
      auto cb = task.onError;
      tasks.erase(it);
      if (cb)
        cb(id, "HTTP error: " + std::to_string(r.status_code) + ", please check your network, or download the offline patch.");
    } else {
      auto cb = task.onSuccess;
      auto result = r.text;
      tasks.erase(it);
      if (cb)
        cb(id, result);
    }
  }).detach();
}

bool DownloadManager::GetProgress(int id, int &outProgress) {
  std::lock_guard<std::mutex> lock(mtx);
  auto it = tasks.find(id);
  if (it == tasks.end())
    return false;
  outProgress = it->second.progress;
  return true;
}

void DownloadManager::Cancel(int id) {
  std::lock_guard<std::mutex> lock(mtx);
  tasks.erase(id);
}
