#include "Timer.h"
#include <functional>
void Timer::Restart() {
  passedTime = 0;
  shotted = false;
}
void Timer::SetWaitTime(int ms) { waitTime = ms; }
void Timer::SetOneShot(bool val) { oneShot = val; }
void Timer::SetCallback(std::function<void()> val) { callback = val; }
void Timer::Pause() { paused = true; }
void Timer::Resume() { paused = false; }
void Timer::Update(int delta) {
  if (paused)
    return;
  passedTime += delta;
  if (passedTime >= waitTime) {
    if ((!oneShot || (oneShot && !shotted)) && callback)
      callback();
    shotted = true;
    passedTime = 0;
  }
}