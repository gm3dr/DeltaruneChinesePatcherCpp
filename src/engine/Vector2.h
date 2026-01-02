#pragma once
class Vector2 {
public:
  Vector2() = default;
  ~Vector2() = default;
  Vector2(float x, float y);
  Vector2 operator+(const Vector2 &) const;
  Vector2 operator-(const Vector2 &) const;
  Vector2 operator*(float) const;
  void operator+=(const Vector2 &);
  void operator-=(const Vector2 &);
  void operator*=(float);
  float Length() const;
  Vector2 Normalize() const;

private:
  float x{0};
  float y{0};
};