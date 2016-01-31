#include "Components/AnimationComponent.hpp"

#include "Animation/Animation.hpp"
#include "Core/Logger.hpp"

#include <algorithm>

namespace dm {

AnimationSystem* AnimationComponent::s_animations = nullptr;

size_t AnimationComponent::animate(const std::string& name, const float speed,
                                   const bool loop,
                                   std::function<void(Entity*)> callback) {
    size_t animation_id =
        s_animations->start_animation(name, speed, loop, callback);
    animations.emplace_back(animation_id);
    return animation_id;
}

void AnimationComponent::stop(const size_t id) {
    auto found = std::find(animations.begin(), animations.end(), id);

    if (found != animations.end()) {
        s_animations->stop_animation(id);
    }
}

} /* namespace dm */
