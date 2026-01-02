#include "Camera.h"
#include "Vector2.h"
Camera::Camera() {
  shakeTimer.SetOneShot(true);
  shakeTimer.SetCallback([this] { shaking = false; });
}
Vector2 Camera::GetPosition() const { return position; }
void Camera::Reset() { position = {0, 0}; }
void Camera::Update(int delta) { 
    shakeTimer.Update(delta);
    if (shaking) {
        position.x = (-50.0 + rand() % 100) / 50.0 * shakeStrength;
        position.y = (-50.0 + rand() % 100) / 50.0 * shakeStrength;
    }
 }
void Camera::Shake(float strength, int duration) {
  shaking = true;
  shakeStrength = strength;
  shakeTimer.SetWaitTime(duration);
  shakeTimer.Restart();
}