#pragma once

#include <iostream>
#include <cmath>

template<typename T>
class Vec2 {
 public:
// constructors
  Vec2() {}

  Vec2(T x, T y) : x(x), y(y) {
  }

  Vec2(const Vec2& other_copy) :
    x(other_copy.x), y(other_copy.y) {}

  template<typename OtherT>
  Vec2(const Vec2<OtherT>& other_copy) :
    x(other_copy.x), y(other_copy.y) {}

  Vec2(Vec2&& other_move) :
    x(other_move.x), y(other_move.y) {
  }

  template<typename OtherT>
  Vec2(Vec2<OtherT>&& other_move) :
    x(other_move.x), y(other_move.y) {
  }

  Vec2& operator=(const Vec2& other_copy) {
    if (this == &other_copy) {
      return *this;
    }

    x = other_copy.x;
    y = other_copy.y;

    return *this;
  }

  template<typename OtherT>
  Vec2& operator=(const Vec2<OtherT>& other_copy) {
    x = other_copy.x;
    y = other_copy.y;

    return *this;
  }

  Vec2& operator=(Vec2&& other_move) {
    if (this == &other_move) {
      return *this;
    }

    x = other_move.x;
    y = other_move.y;

    return *this;
  }

  template<typename OtherT>
  Vec2& operator=(Vec2<OtherT>&& other_move) {
    x = other_move.x;
    y = other_move.y;

    return *this;
  }

  ~Vec2() {}

  double Length() const {
    return sqrt(x * x + y * y);
  }

  T DistTo(const Vec2& other) const {
    return std::sqrt(SqrDistTo(other));
  }

  T SqrDistTo(const Vec2& other) const {
    return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
  }

  Vec2 GetProjection(const Vec2& direction) {
    Vec2 dir_normalized = direction;
    dir_normalized.Normalize();
    return dir_normalized * (*this * dir_normalized);
  }

  void Normalize() {
    double length = Length();
    if (length) {
      x /= length;
      y /= length;
    }
  }

  Vec2<T> GetNormalized() {
    auto copy = *this;
    copy.Normalize();
    return copy;
  }

  void Resize(double new_length) {
    Normalize();
    (*this) *= new_length;
  }

  void Reflect(const Vec2& normal) {
    (*this) = 2 * (normal * *this) * normal - (*this);
  }

  double Angle() const {
    return atan2(y, x);
  }

  void Rotate(const double d_phi) {
    double length = Length();
    double angle = Angle() + d_phi;
    x = cos(angle) * length;
    y = sin(angle) * length;
  }

  template<typename OtherT>
  Vec2& operator+=(const Vec2<OtherT>& right) {
    x += right.x;
    y += right.y;
    return *this;
  }

  template<typename OtherT>
  Vec2& operator-=(const Vec2<OtherT>& right) {
    x -= right.x;
    y -= right.y;
    return *this;
  }

  template<typename OtherT>
  Vec2& operator*=(const OtherT scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vec2 operator-() const {
    return Vec2(-x, -y);
  }

  bool operator==(const Vec2& right) const {
    return x == right.x && y == right.y;
  }

  template<typename OtherT>
  Vec2 operator+(const Vec2<OtherT>& right) const {
    return Vec2(
      x + right.x,
      y + right.y
    );
  }

  template<typename OtherT>
  Vec2 operator-(const Vec2<OtherT>& right) const {
    return Vec2(
      x - right.x,
      y - right.y
    );
  }

  Vec2 operator * (const double scalar) const {
    return Vec2(
      x * scalar,
      y * scalar
    );
  }

  double operator*(const Vec2& right) const {
    return x * right.x +
           y * right.y;
  }

  void Print() const {
    std::cout << "x: " << x << ", y: " << y << '\n';
  }

 public:
  T x = 0;
  T y = 0;

};

template<typename T>
Vec2<T> operator * (const double scalar, const Vec2<T>& vector) {
  Vec2<T> res = vector;
  res *= scalar;
  return res;
}

template<typename T>
double Vec2Cos(const Vec2<T>& first, const Vec2<T>& second) {
  T numerator = first * second;
  if (!numerator)
    return 0;
  return numerator / (first.Length() * second.Length());
}

template<typename T>
double Vec2Sin(const Vec2<T>& first, const Vec2<T>& second) {
  T cos = Vec2Cos(first, second);
  return sqrt(1 - cos * cos);
}

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

