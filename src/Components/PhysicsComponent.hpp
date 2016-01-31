#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "Math/Math.hpp"

#include "Core/Component.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector3.hpp"

#define TICK_TIME 16

namespace dm {

struct AABB {
    Vec3f min;
    Vec3f max;
};

dm_start_component(PhysicsComponent);

PhysicsComponent()
    : scale(1.0f),
      mass(1.0f),
      max_velocity(2.0f),
      friction(1.0f),
      velocity(0.0f),
      acceleration(0.0f),
      position(0.0f),
      size(0.0f),
      transform(1.0f),
      rotation(),
      rotation_origin(0.0f),
      scale_origin(0.0f) {}

inline void set_size(const Vec3f& size) {
    this->size = size;
    pivot = this->size.x * 0.5f;
}

Vec3f scale;

float mass;
float max_velocity;
float friction;

Vec3f velocity;
Vec3f acceleration;
Vec3f position;
Vec3f size;

Matrix4f transform;
Quaternion rotation;
Vec3f rotation_origin;
Vec3f scale_origin;

AABB aabb;

Vec3f pivot;

dm_end_component(PhysicsComponent);
}

#endif
