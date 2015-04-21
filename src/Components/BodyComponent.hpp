#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include "Core/Component.hpp"
#include "Core/Entity.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector2.hpp"

#include "TypeId/Any.hpp"

#include <stack>
#include <functional>

namespace dm {

  const Vec3f TILE_SIZE {
    32.0f / 1280.0f,
      32.0f / 1280.0f,
      32.0f / 1280.0f
      };

  struct Command {

    enum class Type {
      MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	GO_TO
	};

    Type type;
    Any data;
    std::function<void()> callback;

  };

  enum class TileType {
    TERRAIN,
      WALL
      };

  enum class Direction {
    LEFT,
      UP,
      RIGHT,
      DOWN
      };

  struct Tile {
    bool free = true;
    TileType type;
    Entity * entity = nullptr;
  };

  dm_start_component(BodyComponent);

  BodyComponent() : position(0,0), dirty(true), lock(false) {}

  Vec2i position;
  bool dirty;
  bool lock;
  bool ghost = false;
  Direction direction = Direction::RIGHT;

  void execute(const Command::Type& type, const Any& data = nullptr,
	       const std::function<void()> callback = nullptr)
  {
    commands.push({type, data, callback});
  }

  void pop_command()
  {
    if(commands.top().callback) commands.top().callback();
    commands.pop();
  }

  std::stack<Command> commands;

  dm_end_component(BodyComponent);
   
} /* namespace dm */

#endif /* end of include guard: BODYCOMPONENT_HPP */
