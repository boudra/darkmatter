#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "Math/Math.hpp"

namespace dm {

template <typename T>
class Vec3;

template <typename T>
class Vec2 {
   public:
    Vec2() : x(0), y(0){};
    Vec2(T x) : x(x), y(x) {}
    Vec2(T x, T y) : x(x), y(y){};

    template <typename other_type>
    Vec2(const Vec2<other_type> &other)
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    T x, y;

    inline friend std::ostream &operator<<(std::ostream &stream,
                                           const Vec2<T> &vec) {
        return stream << '{' << vec.x << ',' << vec.y << '}';
    }

    inline Vec2<T> operator+(const Vec2<T> &vec) const {
        return Vec2<T>(x + vec.x, y + vec.y);
    }

    inline Vec2<T> operator-(const Vec2<T> &vec) const {
        return Vec2<T>(x - vec.x, y - vec.y);
    }

    inline Vec2<T> operator-() const { return Vec2<T>(-x, -y); }

    inline Vec2<T> operator+(const T val) const {
        return Vec2<T>(x + val, y + val);
    }

    inline Vec2<T> operator-(const T val) const {
        return Vec2<T>(x - val, y - val);
    }

    inline Vec2<T> operator*(const T val) const {
        return Vec2<T>(x * val, y * val);
    }

    template <typename U>
    inline Vec2<U> operator*(const U val) const {
        return Vec2<U>(x * val, y * val);
    }

    inline Vec2<T> operator/(const T val) const {
        return Vec2<T>(x / val, y / val);
    }

    inline Vec2<T> operator*(const Vec2<T> &vec) const {
        return Vec2<T>(x * vec.x, y * vec.y);
    }

    inline Vec2<T> operator/(const Vec2<T> &vec) const {
        return Vec2<T>(x / vec.x, y / vec.y);
    }

    inline const Vec2<T> &operator+=(const Vec2<T> &vec) {
        return *this = *this + vec;
    }

    inline const Vec2<T> &operator-=(const Vec2<T> &vec) {
        return *this = *this - vec;
    }

    inline const Vec2<T> &operator*=(const Vec2<T> &vec) {
        return *this = *this * vec;
    }

    inline const Vec2<T> &operator=(const Vec2<T> &vec) {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    inline bool operator>(const Vec2<T> &vec) const {
        return (x > vec.x && y > vec.y);
    }

    inline bool operator<(const Vec2<T> &vec) const { return !((*this) > vec); }

    inline bool operator>(const T value) const {
        return (x > value && y > value);
    }

    inline bool operator<(const T value) const { return !((*this) > value); }

    inline float length() const { return sqrt((x * x) + (y * y)); }

    inline const Vec2<T> normalize() const {
        float l = length();
        assert(l > 0.0f || l < 0.0f);
        return (*this / l);
    }

    inline Vec2<T> abs() const { return Vec2<T>(std::abs(x), std::abs(y)); }
};

typedef Vec2<int32_t> Vec2i;
typedef Vec2<float> Vec2f;
} /* namespace dm  */

#endif /* end of include guard: VECTOR2_HPP */
