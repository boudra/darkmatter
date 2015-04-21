#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Core/System.hpp"
#include "Math/Matrix4.hpp"
#include "Core/Entity.hpp"
#include "Event/Event.hpp"

namespace dm {
   
  struct PhysicsComponent;

  class Camera : public SystemBase {
   
  public:

    Camera();
    ~Camera();

    void render(float interpolation);
    void update();

    bool initialize();

    void lock(Entity* e);

    bool locked() const {
      return m_lockedEntity != nullptr;
    }

    void button_pressed(const KeyboardEvent& e);
    void button_released(const KeyboardEvent& e);
    void mouse_wheel(const MouseWheelEvent& e);

    const Matrix4f& view() const {
      return m_view;
    }

    const Vec3f& position() const;
    const Vec3f& target_position() const;
    Vec3f& position();

    Entity& target() {
      return *m_lockedEntity;
    }

    const float zoom() {
      return m_zoom;
    }

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
   

} /* namespace dm */


#endif /* end of include guard: CAMERA_HPP */
