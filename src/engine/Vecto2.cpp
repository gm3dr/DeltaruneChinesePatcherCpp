#include "Vector2.h"
#include <cmath>
Vector2::Vector2(float x, float y) : x(x), y(y) {}
Vector2 Vector2::operator+(const Vector2 &vec) const {
  return Vector2{x + vec.x, y + vec.y};
}
Vector2 Vector2::operator-(const Vector2 &vec) const {
  return Vector2{x - vec.x, y - vec.y};
}
Vector2 &Vector2::operator+=(const Vector2 &vec) {
  x += vec.x;
  y += vec.y;
  return *this;
}
Vector2 &Vector2::operator-=(const Vector2 &vec) {
  x -= vec.x;
  y -= vec.y;
  return *this;
}
Vector2 Vector2::operator*(float val) const {
  return Vector2(val * x, val * y);
}
Vector2 &Vector2::operator*=(float val) {
  x *= val;
  y *= val;
  return *this;
}
Vector2 &Vector2::operator=(const Vector2 &vec) {
  if (this != &vec) {
    x = vec.x;
    y = vec.y;
  }
  return *this;
}
float Vector2::Length() const { return sqrt(x * x + y * y); }
Vector2 Vector2::Normalize() const {
  float len = Length();
  if (len <= 1e-6f)
    return Vector2(0, 0);
  return Vector2(x / len, y / len);
}
