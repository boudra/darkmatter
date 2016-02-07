#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Core/System.hpp"
#include "Components/AnimationComponent.hpp"

#include <vector>
#include <string>
#include <functional>

namespace dm {

class AnimationSystem : public System {
   public:
    AnimationSystem() {}
    ~AnimationSystem() {}

    void update(GameState&);
    void render(const GameState&) {}

    size_t start_animation(const std::string& name, float speed = 1.0f,
                           bool loop = false,
                           std::function<void(Entity*)> callback = nullptr);

    size_t register_animation(const std::string& name);
    size_t register_animation(const AnimationInfo& info);
    size_t get_animation(const std::string& name);

    void stop_animation(const size_t animation_id);

   private:
    std::vector<AnimationInfo> m_animations;
    std::vector<AnimationInstance> m_animationInstances;
    std::vector<bool> m_free;
};

} /* namespace dm */

#endif /* end of include guard: ANIMATION_HPP */
