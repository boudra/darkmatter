#include "Camera.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Event/EventDispatcher.hpp"

namespace dm {
   
  Camera::Camera() : SystemBase("Camera"),
		     m_lockedEntity(nullptr),
		     m_target{0.0f},
		     m_relativePosition{0.0f, 0.0f, 1.00f},
		     m_velocity{0.0f},
		     m_drag{0.0f},
		     m_zoom{0.5f},
		     m_zoom_velocity{0.0f},
		     m_view {1.0f} {

		     }

  Camera::~Camera() {

  }

  void Camera::render(float interpolation) { 

  }

  void Camera::update() {


    if(m_lockedEntity) {
      PhysicsComponent* m_physics = &m_lockedEntity->component<PhysicsComponent>();
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

    if(speed < 0.005f) {
      m_target.x = std::round(m_target.x * 100.0f) / 100.0f;
      m_target.z = std::round(m_target.z * 100.0f) / 100.0f;
      m_target.y = std::round(m_target.y * 100.0f) / 100.0f;
    }

    m_position = m_target + (m_relativePosition); 

    m_view.look_at(m_position,
		   m_target, 
		   Vec3f{0.0f, 1.0f, 0.0f}
		   );

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

  bool Camera::initialize() {
    s_dispatcher->subscribe(this, &Camera::button_pressed, EventType::KEY_PRESSED);
    s_dispatcher->subscribe(this, &Camera::button_released, EventType::KEY_RELEASED);
    s_dispatcher->subscribe(this, &Camera::mouse_wheel, EventType::MOUSE_WHEEL);
    return true;
  }

  void Camera::lock(Entity* e) {
    m_lockedEntity = e;
  }

  const Vec3f& Camera::position() const {
    return m_position;
  }

  const Vec3f& Camera::target_position() const {
    return m_target;
  }

  Vec3f& Camera::position() {
    return m_position;
  }

} /* namespace dm */
