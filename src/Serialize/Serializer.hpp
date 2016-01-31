#ifndef DESERALIZER_HPP
#define DESERALIZER_HPP

#include "Json/JsonReader.hpp"
#include "Core/Logger.hpp"
#include "Components/AnimationComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Resource/ResourceManager.hpp"

namespace dm {

   class Deserializer {
   public:

      Deserializer() {}
      ~Deserializer() {}

      template <typename value_type>
      bool deserialize(const std::string& filename, value_type& value) {
         JsonReader reader;
         JsonNode root = reader.Parse(filename.c_str());
         return deserialize(root, value);
      }

      bool deserialize(JsonNode& anim, AnimationInfo& info) {

         info.name = anim["name"].Cast<std::string>();
         info.type = anim["type"].Cast<AnimationType>();

         if(info.type >= AnimationType::SpriteTexture) {
            info.data = anim["sprite"].Cast<std::string>();
         }

         for(auto& node : anim["frames"].Children())
         {

            switch(info.type) {

               case AnimationType::SpriteTexture:
               {
                  auto& frame = node.second["frame"];
                  info.frames.push_back(new AnimationFrame<Vec2i>(
                           (float)node.second["time"].Cast<double>() * 1000.0f,
                           deserialize_vec2i(frame)
                  ));
               }
               break;

               case AnimationType::SpriteScale:
               {
                  auto& position = node.second["scale"];
		  Vec2f scale = deserialize_vec2f(position);
                  info.frames.push_back(new AnimationFrame<Vec3f>(
                           (float)node.second["time"].Cast<double>() * 1000.0f,
			   Vec3f{scale, 0.0f}
                  ));
               }
               break;

               default:
                  auto& position = node.second["position"];
                  info.frames.push_back(new AnimationFrame<Vec3f>(
                           (float)node.second["time"].Cast<double>() * 1000.0f,
			   deserialize_vec3f(position)
                  ));
               break;

            };

         }

         return true;

      }

      bool deserialize(JsonNode& root, Entity& e)
      {

          JsonNode& components = root["components"];

	  Log::debug("Deserializing entity %s", root["name"].Cast<std::string>().c_str());

          for(auto& node : components.Children()) {

	    Log::debug("Deserializing component %s", node.first.c_str());

             if(node.first == "render")
             {
                deserialize(node.second, e.assign<Render2d>());
             }
             else if(node.first == "world")
             {
               deserialize(node.second, e.assign<BodyComponent>());
             }
             else if(node.first == "player")
             {
               e.assign<PlayerComponent>();
             }
             else if(node.first == "animation")
             {
               e.assign<AnimationComponent>();
               //deserialize(node.second, e.assign<AnimationComponent>());
             }
             else if(node.first == "physics")
             {
               deserialize(node.second, e.assign<PhysicsComponent>());
             }
             else
             {
               assert(false);
             }

          }

          return true;
      }

      bool deserialize(JsonNode& root, AnimationComponent& animation)
      {
         for(auto& node : root.Children())
         {
            auto& animation_info = node.second;
            if(node.first == "empty") return true;
            animation.animate(node.first, (float)animation_info["speed"].Cast<double>(),
                  animation_info["loop"].Cast<bool>());
         }
         return true;
      }

      bool deserialize(JsonNode& root, BodyComponent& body)
      {
         body.position = deserialize_vec2i(root["position"]);
         return true;
      }

      bool deserialize(JsonNode& root, Render2d& render)
      {

         render.shadow = root["shadow"].Cast<bool>();
         render.billboard = root["billboard"].Cast<bool>();

         for(auto& node : root["sprites"].Children())
         {
            auto& sprite = node.second;
            Texture* texture = m_resource_manager->get_texture(m_resource_manager->load_texture("data/" + sprite["texture"].Cast<std::string>()));

            Sprite& new_sprite = render.add_sprite(node.first,
                  Vec3f {deserialize_vec2f(sprite["position"]),
                  (float)sprite["depth"].Cast<int>() / 1000.0f},
                  deserialize_vec2f(sprite["size"]), texture->id());

            new_sprite.crop.position = deserialize_vec2i(sprite["crop"]["0"]);
            new_sprite.crop.size = deserialize_vec2i(sprite["crop"]["1"]);

            new_sprite.crop.position = new_sprite.crop.position / texture->sizef();
            new_sprite.crop.size = new_sprite.crop.size / texture->sizef();

            texture->set_wrap_mode(Texture::Repeat, Texture::Repeat);

            texture->bind();
            texture->setup();
            texture->release();

         }

         return true;
      }


      bool deserialize(JsonNode& root, PhysicsComponent& physics)
      {
         physics.set_size(deserialize_vec3f(root["size"]) * TILE_SIZE);
         return true;
      }

      Vec3f deserialize_vec3f(JsonNode& array) {
         return {
            (float)array[0].Cast<double>(),
            (float)array[1].Cast<double>(),
            (float)array[2].Cast<double>()
         };
      }

      Vec2f deserialize_vec2f(JsonNode& array) {
         return {
            (float)array[0].Cast<double>(),
            (float)array[1].Cast<double>()
         };
      }

      Vec2i deserialize_vec2i(JsonNode& array) {
         return {
            array[0].Cast<int>(),
            array[1].Cast<int>()
         };
      }

   private:

      friend Engine;
      static ResourceManager * m_resource_manager;

   };


} /* namespace dm */


#endif /* end of include guard: DESERALIZER_HPP */
