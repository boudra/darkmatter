#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Math/Matrix4.hpp"

namespace dm  {

class Quaternion {
  public:

   Quaternion() : x(0), y(0), z(0), w(1.f) {}
   Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
   Quaternion(const Quaternion& other)
       : x(other.x), y(other.y), z(other.z), w(other.w) {}

   const Quaternion& operator=(const Quaternion& other) {
      x = other.x;
      y = other.y;
      z = other.z;
      w = other.w;
      return *this;
   }

   const Quaternion operator*(const Quaternion& other) const {
      return Quaternion(
          (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y),
          (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z),
          (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x),
          (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z));
   }

   const Quaternion& operator*=(const Quaternion& other) {
      return *this = *this * other;
   }

   const Quaternion& rotate(float angle, const Vec3f& axis) {
      const float half = 0.5f * angle;
      const float sin = sinf(half);
      w = cosf(half);
      x = sin * axis.x;
      y = sin * axis.y;
      z = sin * axis.z;
      return *this;
   }

   void get_matrix(Matrix4f& matrix) const {

      float qxx(x * x);
      float qyy(y * y);
      float qzz(z * z);
      float qxz(x * z);
      float qxy(x * y);
      float qzw(z * w);
      float qyw(y * w);
      float qzy(z * y);
      float qxw(x * w);

      matrix.identity();

      matrix[0] = 1.0f - 2.0f * (qyy + qzz); 
      matrix[4] = 2.0f * (qxy + qzw);
      matrix[8] = 2.0f * (qxz - qyw);

      matrix[1] = 2.0f * (qxy - qzw);
      matrix[5] = 1.0f - 2.0f * (qxx + qzz);
      matrix[9] = 2.0f * (qzy + qxw);

      matrix[2] = 2.0f * (qxz + qyw);
      matrix[6] = 2.0f * (qzy - qxw);
      matrix[10] = 1.0f - 2.0f * (qxx + qyy);

      matrix[3] = 0.0f;
      matrix[7] = 0.0f;
      matrix[11] = 0.0f;

      matrix[12] = 0.0f;
      matrix[13] = 0.0f;
      matrix[14] = 0.0f;
      matrix[15] = 1.0f;
   }

   void set_matrix(Matrix4f& matrix) const {

      float qxx(x * x);
      float qyy(y * y);
      float qzz(z * z);
      float qxz(x * z);
      float qxy(x * y);
      float qzw(z * w);
      float qyw(y * w);
      float qzy(z * y);
      float qxw(x * w);
      

      matrix[0] = 1.0f - 2.0f * (qyy + qzz); 
      matrix[4] = 2.0f * (qxy + qzw);
      matrix[8] = 2.0f * (qxz - qyw);

      matrix[1] = 2.0f * (qxy - qzw);
      matrix[5] = 1.0f - 2.0f * (qxx + qzz);
      matrix[9] = 2.0f * (qzy + qxw);

      matrix[2] = 2.0f * (qxz + qyw);
      matrix[6] = 2.0f * (qzy - qxw);
      matrix[10] = 1.0f - 2.0f * (qxx + qyy);

   }

   float x, y, z, w;
};

} /* namespace dm  */

#endif /* end of include guard: QUATERNION_HPP */
