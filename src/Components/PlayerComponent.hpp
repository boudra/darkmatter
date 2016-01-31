#ifndef PLAYER_COMPONENT_HPP
#define PLAYER_COMPONENT_HPP

#include "Core/Component.hpp"
#include "Math/Vector3.hpp"

namespace dm {

class PlayerManager;

dm_start_component(PlayerComponent);

PlayerComponent()
    : m_speed(.004f, 0.002f, 0.004f), m_direction(1.0f, 0.0f, 0.0f) {}

const Vec3f& GetSpeed() const { return m_speed; }
void SetSpeed(const Vec3f& speed) { m_speed = speed; }

friend PlayerManager;

Vec3f m_speed;
Vec3f m_direction;

dm_end_component(PlayerComponent);
}

#endif
