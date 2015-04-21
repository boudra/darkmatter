#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/Rectangle.hpp"

namespace dm {

class Matrix4f {

  public:

   Matrix4f() {}

   Matrix4f(float x) {
      this->identity();
   }

   template <class... T>
   inline constexpr Matrix4f(float first, T... next)
       : m{first, next...} {}

   Matrix4f& transpose() {
      std::swap(m[1], m[4]);
      std::swap(m[2], m[8]);
      std::swap(m[3], m[12]);
      std::swap(m[6], m[9]);
      std::swap(m[7], m[13]);
      std::swap(m[11], m[14]);
      return *this;
   }

   Matrix4f transpose() const {
      Matrix4f matrix(*this);
      return matrix.transpose();
   }

   bool inverse() {

      float inv[16], det;
      int i;

      inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] -
               m[9] * m[6] * m[15] + m[9] * m[7] * m[14] +
               m[13] * m[6] * m[11] - m[13] * m[7] * m[10];

      inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] +
               m[8] * m[6] * m[15] - m[8] * m[7] * m[14] -
               m[12] * m[6] * m[11] + m[12] * m[7] * m[10];

      inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] -
               m[8] * m[5] * m[15] + m[8] * m[7] * m[13] +
               m[12] * m[5] * m[11] - m[12] * m[7] * m[9];

      inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] +
                m[8] * m[5] * m[14] - m[8] * m[6] * m[13] -
                m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

      inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] +
               m[9] * m[2] * m[15] - m[9] * m[3] * m[14] -
               m[13] * m[2] * m[11] + m[13] * m[3] * m[10];

      inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] -
               m[8] * m[2] * m[15] + m[8] * m[3] * m[14] +
               m[12] * m[2] * m[11] - m[12] * m[3] * m[10];

      inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] +
               m[8] * m[1] * m[15] - m[8] * m[3] * m[13] -
               m[12] * m[1] * m[11] + m[12] * m[3] * m[9];

      inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] -
                m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +
                m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

      inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
               m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];

      inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] +
               m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] +
               m[12] * m[3] * m[6];

      inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] -
                m[4] * m[1] * m[15] + m[4] * m[3] * m[13] +
                m[12] * m[1] * m[7] - m[12] * m[3] * m[5];

      inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] +
                m[4] * m[1] * m[14] - m[4] * m[2] * m[13] -
                m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

      inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] +
               m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] +
               m[9] * m[3] * m[6];

      inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
               m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];

      inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] +
                m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] +
                m[8] * m[3] * m[5];

      inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
                m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

      det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

      if (det == 0) return false;

      det = 1.0 / det;

      for (i = 0; i < 16; i++) m[i] = inv[i] * det;

      return true;
   }

   const float* data() const { return &m[0]; }

   Matrix4f& identity() {
      m[0] = m[5] = m[10] = m[15] = 1.0f;
      m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] =
          m[13] = m[14] = 0.0f;
      return *this;
   }

   Matrix4f& translate(const Vec3f& v) { return translate(v.x, v.y, v.z); }

   Matrix4f& translate(const Vec2f& v) { return translate(v.x, v.y, 0.0f); }

   Matrix4f& translate(const float x, const float y, const float z) {

      m[3] += m[0] * x + m[1] * y + m[2] * z;
      m[7] += m[4] * x + m[5] * y + m[6] * z;
      m[11] += m[8] * x + m[9] * y + m[10] * z;
      m[15] += m[12] * x + m[13] * y + m[14] * z;

      return *this;
   }

   Matrix4f& scale(const Vec3f& v) { return scale(v.x, v.y, v.z); }

   Matrix4f& scale(const Vec2f& v) { return scale(v.x, v.y, 1.0f); }

   Matrix4f& scale(const float x, const float y, const float z) {

      Matrix4f res(1.0f);

      res[0] = x;
      res[5] = y;
      res[10] = z;

      return *this = *this * res;
   }

   const Matrix4f& rotate(const float& angle, const Vec3f& r) {

      const float c = cos(angle);
      const float s = sin(angle);

      Matrix4f t;

      t.m[0] = (r.x * r.x) * (1.0f - c) + c;
      t.m[1] = (r.x * r.y) * (1.0f - c) - r.z * s;
      t.m[2] = (r.x * r.z) * (1.0f - c) + r.y * s;
      t.m[3] = 0.0f;

      t.m[4] = (r.y * r.x) * (1.0f - c) + r.z * s;
      t.m[5] = (r.y * r.y) * (1.0f - c) + c;
      t.m[6] = (r.y * r.z) * (1.0f - c) - r.x * s;
      t.m[7] = 0.0f;

      t.m[8] = (r.x * r.z) * (1.0f - c) - r.y * s;
      t.m[9] = (r.y * r.z) * (1.0f - c) + r.x * s;
      t.m[10] = (r.z * r.z) * (1.0f - c) + c;
      t.m[11] = 0.0f;

      t.m[12] = 0.0f;
      t.m[13] = 0.0f;
      t.m[14] = 0.0f;
      t.m[15] = 1.0f;

      return *this = t * *this;
   }

   const Matrix4f& ortho(const Rectangle& plane, const Vec2f& depth) {
      m[0] = 2.0f / (plane.size.x - plane.position.x);
      m[1] = 0.0f;
      m[2] = 0.0f;
      m[3] = -((plane.size.x + plane.position.x) /
               (plane.size.x - plane.position.x));
      m[4] = 0.0f;
      m[5] = 2.0f / (plane.size.y - plane.position.y);
      m[6] = 0.0f;
      m[7] = -((plane.size.y + plane.position.y) /
               (plane.size.y - plane.position.y));
      m[8] = 0.0f;
      m[9] = 0.0f;
      m[10] = -2.0f / (depth.y - depth.x);
      m[11] = -((depth.y + depth.x) / (depth.y - depth.x));
      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;
      return *this;
   }

   const Matrix4f& perspective(const float& aspect, const float near,
                               const float far, const float fov) {
      float scale = tan((M_PI / 180.0f) * (fov * 0.5));
      m[0] = 1.0f / (aspect * scale);
      m[5] = 1.0f / scale;
      m[10] = -(far + near) / (far - near);
      m[11] = -(2.0f * far * near) / (far - near);
      m[14] = -1.0f;
      m[15] = 0.0f;
      m[1] = 0.0f;
      m[2] = 0.0f;
      m[3] = 0.0f;
      m[4] = 0.0f;
      m[6] = 0.0f;
      m[7] = 0.0f;
      m[8] = 0.0f;
      m[9] = 0.0f;
      m[12] = 0.0f;
      m[13] = 0.0f;
      return *this;
   }

   const Matrix4f& look_at(const Vec3f& eye, const Vec3f& target,
                          const Vec3f& up) {

      const Vec3f& f = Vec3f(target - eye).normalize();
      const Vec3f& s = f.cross(up).normalize();
      const Vec3f& u = s.cross(f);

      m[0] = s.x;
      m[1] = s.y;
      m[2] = s.z;

      m[4] = u.x;
      m[5] = u.y;
      m[6] = u.z;

      m[8] = -f.x;
      m[9] = -f.y;
      m[10] = -f.z;

      m[3] = -s.dot(eye);
      m[7] = -u.dot(eye);
      m[11] = f.dot(eye);

      m[12] = 0.0f;
      m[13] = 0.0f;
      m[14] = 0.0f;
      m[15] = 1.0f;

      return *this;
   }

   inline Matrix4f operator*(const Matrix4f& n) const {

      return Matrix4f(
          m[0] * n[0] + m[1] * n[4] + m[2] * n[8] + m[3] * n[12],
          m[0] * n[1] + m[1] * n[5] + m[2] * n[9] + m[3] * n[13],
          m[0] * n[2] + m[1] * n[6] + m[2] * n[10] + m[3] * n[14],
          m[0] * n[3] + m[1] * n[7] + m[2] * n[11] + m[3] * n[15],
          m[4] * n[0] + m[5] * n[4] + m[6] * n[8] + m[7] * n[12],
          m[4] * n[1] + m[5] * n[5] + m[6] * n[9] + m[7] * n[13],
          m[4] * n[2] + m[5] * n[6] + m[6] * n[10] + m[7] * n[14],
          m[4] * n[3] + m[5] * n[7] + m[6] * n[11] + m[7] * n[15],
          m[8] * n[0] + m[9] * n[4] + m[10] * n[8] + m[11] * n[12],
          m[8] * n[1] + m[9] * n[5] + m[10] * n[9] + m[11] * n[13],
          m[8] * n[2] + m[9] * n[6] + m[10] * n[10] + m[11] * n[14],
          m[8] * n[3] + m[9] * n[7] + m[10] * n[11] + m[11] * n[15],
          m[12] * n[0] + m[13] * n[4] + m[14] * n[8] + m[15] * n[12],
          m[12] * n[1] + m[13] * n[5] + m[14] * n[9] + m[15] * n[13],
          m[12] * n[2] + m[13] * n[6] + m[14] * n[10] + m[15] * n[14],
          m[12] * n[3] + m[13] * n[7] + m[14] * n[11] + m[15] * n[15]);
   }

   inline Vec3f operator*(const Vec3f& rhs) const {
      return Vec3f(m[0] * rhs.x + m[1] * rhs.y + m[2] * rhs.z + m[3] * 1.0f,
                   m[4] * rhs.x + m[5] * rhs.y + m[6] * rhs.z + m[7] * 1.0f,
                   m[8] * rhs.x + m[9] * rhs.y + m[10] * rhs.z + m[11] * 1.0f);
   }

   inline Vec4f operator*(const Vec4f& rhs) const {
      return Vec4f(
          m[0] * rhs.x + m[1] * rhs.y + m[2] * rhs.z + m[3] * rhs.w,
          m[4] * rhs.x + m[5] * rhs.y + m[6] * rhs.z + m[7] * rhs.w,
          m[8] * rhs.x + m[9] * rhs.y + m[10] * rhs.z + m[11] * rhs.w,
          m[12] * rhs.x + m[13] * rhs.y + m[14] * rhs.z + m[15] * rhs.w);
   }

   inline Vec2f operator*(const Vec2f& rhs) const {
      Vec3f v = *this * Vec3f(rhs);
      return Vec2f(v.x, v.y);
   }

   inline const float& operator[](const size_t& index) const {
      return m[index];
   }
   inline float& operator[](const size_t& index) { return m[index]; }

   inline friend std::ostream& operator<<(std::ostream& stream,
                                          const Matrix4f& m) {
      return stream << std::endl << std::right << std::fixed
                    << std::setprecision(5) << '{' << std::setw(8) << m[0]
                    << ',' << std::setw(8) << m[1] << ',' << std::setw(8)
                    << m[2] << ',' << std::setw(8) << m[3] << '}' << std::endl << '{'
                    << std::setw(8) << m[4] << ',' << std::setw(8) << m[5]
                    << ',' << std::setw(8) << m[6] << ',' << std::setw(8)
                    << m[7] << '}' << std::endl << '{' << std::setw(8) << m[8]
                    << ',' << std::setw(8) << m[9] << ',' << std::setw(8)
                    << m[10] << ',' << std::setw(8) << m[11] << '}' << std::endl
                    << '{' << std::setw(8) << m[12] << ',' << std::setw(8)
                    << m[13] << ',' << std::setw(8) << m[14] << ','
                    << std::setw(8) << m[15] << '}';
   }

   float m[16];
};

} /* namespace dm */

#endif /* end of include guard: MATRIX4_HPP */
