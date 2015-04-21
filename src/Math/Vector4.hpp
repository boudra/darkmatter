#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include "Math/Math.hpp"

namespace dm {

template <typename T>
class Vec4Base {
  public:
   Vec4Base() : x(0), y(0), z(0), w(0) {}
   Vec4Base(T x) : x(x), y(x), z(x), w(x) {}
   Vec4Base(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
   Vec4Base(const Vec4Base<T>& other)
       : x(other.x), y(other.y), z(other.z), w(other.w) {}

   const Vec4Base<T> operator+(const Vec4Base<T>& other) const {
      return Vec4Base<T>(x + other.x, y + other.y, z + other.z, w + other.w);
   }

   const Vec4Base<T> operator-(const Vec4Base<T>& other) const {
      return Vec4Base<T>(x - other.x, y - other.y, z - other.z, w - other.w);
   }

   template <typename U>
   const Vec4Base<T> operator*(const U& rhs) const {
      return Vec4Base<T>(x * rhs, y * rhs, z * rhs, w * rhs);
   }

   const Vec4Base<T>& operator=(const Vec4Base<T>& other) {
      x = other.x;
      y = other.y;
      z = other.z;
      w = other.w;
      return *this;
   }

   template <typename U>
   Vec4Base<U> operator/(const U value) {
      return Vec4Base<U>(x / value, y / value, z / value, w / value);
   }

   template <typename U>
   Vec4Base<U> operator*(const U value) {
      return Vec4Base<U>(x * value, y * value, z * value, w * value);
   }
  
   const Vec4Base<T>& operator+=(const Vec4Base<T>& other) {
      return *this = *this + other;
   }

   inline friend std::ostream& operator<<(std::ostream& stream,
                                          const Vec4Base<T>& vec) {
      return stream << '{' << vec.x << ',' << vec.y << ',' << vec.z <<  ',' << vec.w << '}';
   }

   const float length() const {
      return sqrt(x * x + y * y + z * z + w * w);
   }

   const Vec4Base<T>& normalize() {
      return *this = *this / length();
   }

   T x, y, z, w;
};

typedef Vec4Base<float> Vec4f;
typedef Vec4Base<int> Vec4i;

/* Predefined colors */
namespace Color {
   const Vec4i White(255, 255, 255, 255);
   const Vec4i Black(0, 0, 0, 255);
   const Vec4i Red(255, 0, 0, 255);
   const Vec4i Green(0, 255, 0, 255);
   const Vec4i Blue(0, 0, 255, 255);
   const Vec4i Yellow(255, 255, 0, 255);
}

namespace Colorf {
   const Vec4f White(1.0f, 1.0f, 1.0f, 1.0f);
   const Vec4f Black(0, 0, 0, 1.0f);
   const Vec4f Red(1.0f, 0, 0, 1.0f);
   const Vec4f Green(0, 1.0f, 0, 1.0f);
   const Vec4f Blue(0, 0, 1.0f, 1.0f);
   const Vec4f Yellow(1.0f, 1.0f, 0, 1.0f);
}
} /* namespace dm */

#endif /* end of include guard: VECTOR4_HPP */
