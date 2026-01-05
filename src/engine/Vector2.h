#pragma once
class Vector2 {
public:
  Vector2() = default;
  ~Vector2() = default;
  Vector2(float x, float y);
  Vector2 operator+(const Vector2 &) const;
  Vector2 operator-(const Vector2 &) const;
  Vector2 operator*(float) const;
  Vector2 &operator+=(const Vector2 &);
  Vector2 &operator-=(const Vector2 &);
  Vector2 &operator*=(float);
  Vector2 &operator=(const Vector2 &);
  float Length() const;
  Vector2 Normalize() const;
  float x{0};
  float y{0};
};