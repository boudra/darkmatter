#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include "Core/Component.hpp"

#include "Math/Vector4.hpp"
#include "Math/Rectangle.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"
#include "Core/ObjectPool.hpp"

#include <vector>
#include <map>

namespace dm {

class Texture;
class ProgramShader;

struct Sprite {
    Vec4i color;
    Rectangle crop;
    Vec3f position;
    Vec2f size;
    Quaternion rotation;
    Matrix4f transform;
    Vec3f scale = Vec3f(1.0f);
    Vec3f rotation_origin;
    Vec3f scale_origin;
    size_t texture = 0;
    ProgramShader* shader = nullptr;
    std::string name = "";

    void FlipVertical() {
        crop.position.y = 1.0f - crop.position.y;
        crop.size.y = -crop.size.y;
    }

    void FlipHorizontal() {
        // crop.position.x += crop.size.x;
        // crop.size.x = -crop.size.x;
        scale.x *= -1.0f;
    }

    dm_internal_memory_pool(Sprite);
};

struct Render2d : public Component {
    void set_transform(const Matrix4f& matrix);

    Sprite& add_sprite(const std::string& name, const Vec3f& position,
                       const Vec2f& size, const size_t& texture,
                       const Vec4i& color = Color::White);

    void update_bounding_box();

    Sprite& get_sprite(const std::string& name) { return *(sprites[name]); }

    bool has_sprite(const std::string& name) {
        return sprites.find(name) != sprites.end();
    }

    Render2d()
        : transform(1.0f),
          size(0.0f),
          position(0.0f),
          shadow(true),
          billboard(true) {}

    Matrix4f transform;
    Vec2f size;
    Vec3f position;
    bool shadow;
    bool billboard;

    std::map<std::string, Sprite*> sprites;

    dm_register_component(Render2d);
};
}

#endif
