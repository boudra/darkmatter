#include "Camera.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Event/EventDispatcher.hpp"

namespace dm {

Camera::~Camera() {}

void Camera::update(GameState& state) {
    if (state.is("initial")) {
        state.subscribe("key_down", this, &Camera::button_pressed);
        state.subscribe("key_up", this, &Camera::button_released);
        state.subscribe("mouse_wheel", this, &Camera::mouse_wheel);

        state.set("view_matrix", Matrix4f());
        state.set("projection_matrix", Matrix4f());

    } else {
        if (m_lockedEntity) {
            PhysicsComponent* m_physics =
                &m_lockedEntity->component<PhysicsComponent>();
            const Vec3f& size = m_physics->size;
            m_target = m_physics->position + size * 0.5f;
        } else {
            m_target += m_velocity;
            m_velocity = m_velocity * m_drag;
        }

        m_zoom += m_zoom_velocity;
        m_zoom_velocity *= m_zoom_drag;

        m_zoom = max(0.01f, m_zoom);

        const float speed = m_velocity.length();

        if (speed < 0.005f) {
            m_target.x = std::round(m_target.x * 100.0f) / 100.0f;
            m_target.z = std::round(m_target.z * 100.0f) / 100.0f;
            m_target.y = std::round(m_target.y * 100.0f) / 100.0f;
        }

        m_position = m_target + (m_relativePosition);

        m_view.look_at(m_position, m_target, Vec3f{0.0f, 1.0f, 0.0f});

        Matrix4f projection;
        projection.ortho(
            {{-m_zoom, -m_zoom * 0.5625f}, {m_zoom, m_zoom * 0.5625f}},
            {-1.0f, 100.0f});

        state.set("view_matrix", m_view);
        state.set("projection_matrix", projection);
    }
}

void Camera::button_pressed(const KeyboardEvent& e) {
    switch (e.key) {
        case 'w':
            m_velocity.y = 0.01f;
            m_drag.y = 1.0f;
            break;

        case 'a':
            m_velocity.x = -0.01f;
            m_drag.x = 1.0f;
            break;

        case 's':
            m_velocity.y = -0.01f;
            m_drag.y = 1.0f;
            break;

        case 'd':
            m_velocity.x = 0.01f;
            m_drag.x = 1.0f;
            break;

        case 'z':
            m_zoom_velocity = 0.01f;
            m_zoom_drag = 1.0f;
            break;

        case 'x':
            m_zoom_velocity = -0.01f;
            m_zoom_drag = 1.0f;
            break;
    }
}

void Camera::button_released(const KeyboardEvent& e) {
    switch (e.key) {
        case 'w':
        case 's':
            m_drag.y = 0.5f;
            break;

        case 'a':
        case 'd':
            m_drag.x = 0.5f;
            break;

        case 'z':
        case 'x':
            m_zoom_drag = 0.5f;
            break;
    }
}

void Camera::mouse_wheel(const MouseWheelEvent& e) {
    m_zoom_velocity = float(e.delta.y) * -0.01f;
    m_zoom_drag = 0.90f;
}

void Camera::lock(Entity* e) { m_lockedEntity = e; }

const Vec3f& Camera::position() const { return m_position; }

const Vec3f& Camera::target_position() const { return m_target; }

Vec3f& Camera::position() { return m_position; }

} /* namespace dm */
