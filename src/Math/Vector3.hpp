#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "Math/Math.hpp"
#include "Math/Vector2.hpp"

#include <iomanip>

namespace dm {

template <typename T>
class Vec3 {
   public:
    Vec3() : x(0.f), y(0.f), z(0.f) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vec3(const Vec2<T> &v) : x(v.x), y(v.y), z(0) {}
    Vec3(const Vec2<T> &v, const T &z) : x(v.x), y(v.y), z(z) {}

    Vec3(float x, float y) : x(x), y(y), z(0) {}

    Vec3(const T &v) : x(v), y(v), z(v) {}
    Vec3(const Vec3<T> &v) : x(v.x), y(v.y), z(v.z) {}

    Vec3<T> operator*(const Vec3<T> &rhs) const {
        return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    Vec3<T> operator*(const T &rhs) const {
        return Vec3<T>(x * rhs, y * rhs, z * rhs);
    }

    const Vec3<T> &operator*=(const Vec3<T> &rhs) {
        return *this = *this * rhs;
    }

    Vec3<T> operator+(const Vec3<T> &rhs) const {
        return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vec3<T> operator+(const Vec2<T> &rhs) const {
        return Vec3<T>(x + rhs.x, y + rhs.y, z);
    }

    Vec3<T> operator+(const T &rhs) const {
        return Vec3<T>(x + rhs, y + rhs, z + rhs);
    }

    const Vec3<T> &operator+=(const T &rhs) const {
        return *this = *this + rhs;
    }

    const Vec3<T> &operator+=(const Vec3<T> &rhs) {
        return *this = *this + rhs;
    }

    Vec3<T> operator-(const Vec3<T> &rhs) const {
        return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vec3<T> operator-(const Vec2<T> &rhs) const {
        return Vec3<T>(x - rhs.x, y - rhs.y, z);
    }

    Vec3<T> operator-(const T &rhs) const {
        return Vec3<T>(x - rhs, y - rhs, z - rhs);
    }

    const Vec3<T> &operator-=(const T &rhs) const {
        return *this = *this - rhs;
    }

    const Vec3<T> &operator-=(const Vec3<T> &rhs) {
        return *this = *this - rhs;
    }

    Vec3<T> operator-() const { return Vec3<T>(-x, -y, -z); }

    Vec3<T> operator/(const Vec3<T> &rhs) const {
        return Vec3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    Vec3<T> operator/(const T &rhs) const {
        return Vec3<T>(x / rhs, y / rhs, z / rhs);
    }

    const Vec3<T> &operator/=(const Vec3<T> &rhs) {
        return *this = *this / rhs;
    }

    Vec3<T> normalize() const {
        float l = length();
        return (l > 0.0f) ? *this / l : 0.0f;
    }

    float length() const { return sqrt(x * x + y * y + z * z); }

    Vec3<T> cross(const Vec3<T> &rhs) const {
        return Vec3<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                       x * rhs.y - y * rhs.x);
    }

    float dot(const Vec3<T> &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    inline friend std::ostream &operator<<(std::ostream &stream,
                                           const Vec3<T> &vec) {
        return stream << std::fixed << std::setprecision(5) << '{' << vec.x
                      << ',' << vec.y << ',' << vec.z << '}';
    }

    float x, y, z;
};

template <typename T>
Vec3<T> operator/(const T &rhs, const Vec3<T> &v) {
    return Vec3<T>(v.x / rhs, v.y / rhs, v.z / rhs);
}

typedef Vec3<float> Vec3f;

} /* namespace dm */

#endif /* end of include guard: VECTOR3_HPP */
