#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Core/System.hpp"
#include "Math/Matrix4.hpp"
#include "Core/Entity.hpp"
#include "Event/Event.hpp"

namespace dm {

struct PhysicsComponent;

class Camera : public System {
   public:
    Camera()
        : m_lockedEntity(nullptr),
          m_target{0.0f},
          m_relativePosition{0.0f, 0.0f, 1.00f},
          m_velocity{0.0f},
          m_drag{0.0f},
          m_zoom{0.5f},
          m_zoom_velocity{0.0f},
          m_view{1.0f} {}

    ~Camera();

    void render(const GameState& state) {}
    void update(GameState&);

    void lock(Entity* e);

    bool locked() const { return m_lockedEntity != nullptr; }

    void button_pressed(const KeyboardEvent& e);
    void button_released(const KeyboardEvent& e);
    void mouse_wheel(const MouseWheelEvent& e);

    const Matrix4f& view() const { return m_view; }

    const Vec3f& position() const;
    const Vec3f& target_position() const;
    Vec3f& position();

    Entity& target() { return *m_lockedEntity; }

    const float zoom() { return m_zoom; }

   private:
    Entity* m_lockedEntity;

    Vec3f m_position;
    Vec3f m_target;
    Vec3f m_relativePosition;
    Vec3f m_velocity;
    Vec3f m_drag;

    float m_zoom;
    float m_zoom_velocity;
    float m_zoom_drag;

    Matrix4f m_view;
};
dm_internal_register_type(Camera);

} /* namespace dm */

#endif /* end of include guard: CAMERA_HPP */
