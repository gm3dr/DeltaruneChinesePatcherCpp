#pragma once
#include "Timer.h"
#include "Vector2.h"
class Camera {
public:
  Camera();
  ~Camera() = default;
  void Update(int delta);
  Vector2 GetPosition() const;
  void Reset();
  void Shake(float strength = 20, int duration = 500);

private:
  Vector2 position{0, 0};
  Timer shakeTimer;
  bool shaking{false};
  int shakeStrength{0};
};