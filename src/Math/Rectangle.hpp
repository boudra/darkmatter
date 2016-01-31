#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Math/Vector2.hpp"

namespace dm {

class Rectangle {
   public:
    Rectangle() : position(0, 0), size(0, 0) {}
    Rectangle(Vec2f pos, Vec2f size) : position(pos), size(size) {}

    Vec2f position, size;

    inline float top() const { return (position.y + size.y); }
    inline float right() const { return (position.x + size.x); }
    inline float left() const { return position.x; }
    inline float bottom() const { return position.y; }
};

} /* namespace dm  */

#endif /* end of include guard: RECTANGLE_HPP */
