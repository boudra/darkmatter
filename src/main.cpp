#include "Math/Math.hpp"
#include "System/Window.hpp"
#include "Resource/Texture.hpp"
#include "Graphics/Spritebatch.hpp"
#include "Resource/Font.hpp"

#include "Components/RenderComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Components/AnimationComponent.hpp"

#include "Core/Engine.hpp"
#include "Core/EntityManager.hpp"
#include "Core/ComponentManager.hpp"

#include "Serialize/Serializer.hpp"
#include "Json/JsonReader.hpp"

#include "Input/PlayerManager.hpp"
#include "Graphics/Shader.hpp"

#include "Animation/Animation.hpp"
#include "Util/Memory.hpp"

using namespace dm;

dm_register_type(JsonNode);
dm_register_type(Vec2i);

int main()
{
  Engine game;

  game.initialize();

  Entity * character = new Entity();

  Deserializer des;
  des.deserialize("data/entities/capitain.json", *character);
  des.deserialize("data/entities/pirate.json", *(new Entity()));
  des.deserialize("data/entities/doctor.json", *(new Entity()));

  game.start();

  Log::debug("%d", 25);

  return 0;
}
