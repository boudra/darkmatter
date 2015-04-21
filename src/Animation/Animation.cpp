#include "Animation/Animation.hpp"
#include "Serialize/Serializer.hpp"
#include "Core/EntityManager.hpp"

#include "Components/PhysicsComponent.hpp"

#include "Core/Logger.hpp"

#include <algorithm>

namespace dm {
   
AnimationSystem::AnimationSystem() : SystemBase("Animation") {

}

AnimationSystem::~AnimationSystem() {

}

template <typename ValueType>
ValueType interpolate(const AnimationFrameBase& current, const AnimationFrameBase& next, float step)
{
   const auto& currentFrame = static_cast<const AnimationFrame<ValueType>&>(current);
   const auto& nextFrame = static_cast<const AnimationFrame<ValueType>&>(next);
   return (nextFrame.value - currentFrame.value) * step;
}

void animate(AnimationComponent& c, const AnimationInstance& inst, const AnimationInfo& info, const AnimationFrameBase& current, const AnimationFrameBase& next, float step) {
   
      switch(info.type) {

         case AnimationType::PhysicsPosition:
         {
            auto& physics = c.parent->component<PhysicsComponent>();
            const Vec3f& new_pos = physics.position + interpolate<Vec3f>(current, next, (step));
            physics.position = new_pos;
         }
         break;

         case AnimationType::PhysicsScale:
         {
            auto& physics = c.parent->component<PhysicsComponent>();
            const Vec3f& scale = physics.scale;
            const Vec3f& new_scale = scale + interpolate<Vec3f>(current, next, (step));
            physics.scale = new_scale;

         }
         break;

         case AnimationType::SpritePosition:
         {
            auto& render = c.parent->component<Render2d>();
            auto& sprite = render.get_sprite(info.data.Cast<std::string>());
            const Vec3f& new_pos = sprite.position + interpolate<Vec3f>(current, next, (step));
            sprite.position = new_pos;
         }  
         break;

         case AnimationType::SpriteColor:
         {
            auto& render = c.parent->component<Render2d>();
            auto& sprite = render.get_sprite(info.data.Cast<std::string>());
            const Vec4i& new_color = sprite.color + interpolate<Vec4i>(current, next, (step));
            sprite.color = new_color;
         }  
         break;

         case AnimationType::SpriteTexture:
         {
            auto& render = c.parent->component<Render2d>();
            auto& sprite = render.get_sprite(info.data.Cast<std::string>());
            const auto& current_frame = static_cast<const AnimationFrame<Vec2i>&>(current);
            sprite.crop.position = sprite.crop.size * Vec2f{current_frame.value};
         }  
         break;

         case AnimationType::SpriteScale:
         {
            auto& render = c.parent->component<Render2d>();
            auto& sprite = render.get_sprite(info.data.Cast<std::string>());
            const Vec3f& new_scale = sprite.scale + interpolate<Vec3f>(current, next, (step));
            sprite.scale = new_scale;
         }  
         break;

         default:
         break;

      };

}

void AnimationSystem::update() {

  for(auto& c : COMPONENTS(AnimationComponent)) {
   
      for(auto animation = c.animations.begin(); animation != c.animations.end();) {

         AnimationInstance& inst = m_animationInstances[*animation];
         const AnimationInfo& info = m_animations[inst.animation];
         auto& frames = info.frames;

         const AnimationFrameBase& next = *frames[inst.current + 1];
         const AnimationFrameBase& current = *frames[inst.current];

         float step = (16.0f * inst.speed) / (next.time - current.time);

         animate(c, inst, info, current, next, step);

         inst.time += 16.0f * inst.speed;

         if((!inst.backwards && inst.time >= next.time) ||
            (inst.backwards && inst.time <= current.time))
         {
            inst.current += inst.backwards ? -1 : 1;

            if(inst.current == (int32_t)info.frames.size() - 1) {

               if(!inst.loop)
               {
                  /* Adjust destination */
                  if(inst.time > next.time)
                  {
                     animate(c, inst, info, current, next,
                           (next.time - inst.time) / (next.time - current.time));
                  }

                  if(inst.callback) inst.callback(c.parent);

                  m_free[*animation] = true;
                  c.animations.erase(animation);
                  continue;
               }
               else
               {
                  inst.current--;
                  inst.backwards = true;
                  inst.speed *= -1.0f;
               }

            }
            else if(inst.current == -1) {
               inst.current++;
               inst.backwards = false;
               inst.speed *= -1.0f;
            }

         }

         ++animation;
      }

   }
}

void AnimationSystem::render(float interpolation) {

}

size_t AnimationSystem::start_animation(const std::string& name, float speed, bool loop,
      std::function<void(Entity*)> callback) {

   size_t id = 0;
   auto free = std::find(m_free.begin(), m_free.end(), true);

   if(free == m_free.end())
   {
      m_free.push_back(false);
      m_animationInstances.emplace_back();
      id = m_animationInstances.size() - 1;
   }
   else
   {
      id = free - m_free.begin();
      m_free[id] = false; 
   }

   AnimationInstance& inst = m_animationInstances[id];

   inst.speed = speed;
   inst.loop = loop;
   inst.time = 0;
   inst.backwards = false;
   inst.animation = this->get_animation(name);
   inst.current = 0;
   inst.callback = callback;

   return id;

}

size_t AnimationSystem::register_animation(const AnimationInfo& info) {
   m_animations.emplace_back(info);
   return m_animations.size() - 1;
}

size_t AnimationSystem::register_animation(const std::string& name) {
   AnimationInfo info;
   Deserializer deserializer;
   deserializer.deserialize(name, info);
   m_animations.emplace_back(std::move(info));
   return m_animations.size() - 1;
}

size_t AnimationSystem::get_animation(const std::string& name) {

   auto found = std::find_if(m_animations.begin(), m_animations.end(),
   [&name] (const AnimationInfo& info) {
      return info.name == name;
   });

   if(found != m_animations.end()) {
      return found - m_animations.begin();
   }
   else
   {
     Log::error("Animation %s not found.", name.c_str());
      assert(false);
   }

}

bool AnimationSystem::initialize()
{
   AnimationComponent::s_animations = this;
   return true;
}

void AnimationSystem::stop_animation(const size_t animation_id)
{
  AnimationInstance& inst = m_animationInstances[animation_id];
  inst.loop = false;
  inst.speed *= 1.5f;
}


} /* namespace dm */
