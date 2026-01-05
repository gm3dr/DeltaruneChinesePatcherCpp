#pragma once
#include <functional>
class Timer {
public:
  Timer() = default;
  ~Timer() = default;
  void Restart();
  void SetWaitTime(int ms);
  void Pause();
  void Resume();
  void SetOneShot(bool val);
  void SetCallback(std::function<void()> val);
  void Update(int delta);

private:
  int waitTime{0};
  int passedTime{0};
  bool paused{false};
  bool shotted{false};
  bool oneShot{false};
  std::function<void()> callback;
};