#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include "Core/Component.hpp"
#include "Math/Vector3.hpp"
#include "TypeId/Any.hpp"

#include <vector>
#include <functional>

namespace dm {

  enum class AnimationType {
    PhysicsScale,
      PhysicsPosition,
      PhysicsRotation,
      SpriteTexture,
      SpritePosition,
      SpriteRotation,
      SpriteScale,
      SpriteColor
      };

  struct AnimationFrameBase {
    AnimationFrameBase(const float time) : time(time) {}
    float time;
  };

  template <typename ValueType>
  struct AnimationFrame : AnimationFrameBase {
    AnimationFrame(const float time, const ValueType& value)
      : AnimationFrameBase(time), value(value) {}
    ValueType value;
  };

  struct AnimationInfo {
    std::string name;
    AnimationType type;
    std::vector<AnimationFrameBase*> frames;
    Any data;
  };

  struct SpriteAnimation {
    std::string sprite;
  };

  class AnimationSystem;


  dm_start_component(AnimationComponent);
  
  std::vector<size_t> animations;
  static AnimationSystem* s_animations;
  size_t animate(const std::string& name, const float speed, const bool loop,
		 std::function<void(Entity*)> callback = nullptr);
  void stop(const size_t id);

  dm_end_component(AnimationComponent);

  struct AnimationInstance {
    size_t animation;
    int32_t current;
    float speed;
    float time;
    bool backwards;
    bool loop;
    std::function<void(Entity*)> callback;
  };

} /* namespace dm */

#endif /* end of include guard: ANIMATIONCOMPONENT_HPP */
