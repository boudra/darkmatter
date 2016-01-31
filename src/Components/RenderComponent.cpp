#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include "Components/RenderComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Core/EntityManager.hpp"
#include "Resource/Texture.hpp"

namespace dm {

void Render2d::set_transform(const Matrix4f &matrix) {
    transform = matrix;
    for (auto &s : sprites) s.second->transform = matrix;
}

Sprite &Render2d::add_sprite(const std::string &name, const Vec3f &position,
                             const Vec2f &size, const size_t &texture,
                             const Vec4i &color) {
    Sprite *sprite = new Sprite();
    sprites[name] = sprite;
    sprite->name = name;

    sprite->color = color;
    sprite->crop = Rectangle(Vec2f(0.0f), Vec2f(1.0f));
    sprite->position = position;
    sprite->size = size;
    sprite->texture = texture;
    sprite->rotation_origin = Vec3f{0.5f};
    sprite->scale_origin = Vec3f{0.5f};
    sprite->transform = transform;

    update_bounding_box();

    return *sprite;
}

void Render2d::update_bounding_box() {
    for (auto &vk : sprites) {
        Sprite &s = *vk.second;

        position.x = std::min(position.x, s.position.x);
        position.y = std::min(position.y, s.position.y);
        position.z = std::min(position.z, s.position.z);

        size.x = std::max(position.x + size.x, s.position.x + s.size.x);
        size.y = std::max(position.y + size.y, s.position.y + s.size.y);
    }
}

} /* namespace dm */

#endif /* end of include guard: RENDERCOMPONENT_HPPNDERCOMPONENT_H */
