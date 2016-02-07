#include "Graphics/World.hpp"

#include "Core/EntityManager.hpp"
#include "Animation/Animation.hpp"
#include "Core/Engine.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/Cursor.hpp"
#include "Graphics/RenderSystem.hpp"
#include "Resource/ResourceManager.hpp"

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <cstring>
#include <unordered_map>

namespace dm {

void unlock_body(Entity* e) { e->component<BodyComponent>().lock = false; }

bool World::move(BodyComponent& body, int x, int y) {
    if (!m_map(body.position.x + x, body.position.y + y).free) {
        return false;
    }

    this->set_position(body, body.position.x + x, body.position.y + y);
    return true;
}

void World::component_added(const uint32_t component_type, Entity e) {
    // e.component<BodyComponent>().execute(Command::Type::GO_TO, Vec2i
    // {20,20});
}

void World::set_position(BodyComponent& body, int x, int y) {
    if (!body.ghost) {
        Tile& last_position = m_map(body.position);
        Tile& new_position = m_map(x, y);

        last_position.free = true;
        last_position.entity = nullptr;

        new_position.free = false;
        new_position.entity = body.parent;
    }

    body.position.x = x;
    body.position.y = y;
}

Vec3f World::real_position(Vec2i position, Vec3f size) {
    Vec3f positionf{static_cast<float>(position.x),
                    static_cast<float>(position.y), 0.0f};

    Vec3f real = (positionf * TILE_SIZE) + ((TILE_SIZE * 0.5f) - (size * 0.5f));

    // real.y += TILE_SIZE.y * 0.5f - size.y * 0.25f;
    real.z = 0.0001f;

    return real;
}

void World::update(GameState& state) {
    auto& bodies = COMPONENTS(BodyComponent);
    auto& cursors = COMPONENTS(Cursor);

    for (auto& body : bodies) {
        Entity* e = body.parent;
        AnimationComponent* animation =
            e->has_component<AnimationComponent>()
                ? &e->component<AnimationComponent>()
                : nullptr;

        if (!body.lock && !body.dirty && body.commands.size() > 0) {
            Command command = body.commands.top();

            /* TODO: FSM */
            switch (command.type) {
                case Command::Type::MOVE_UP: {
                    if (this->move(body, 0, 1)) {
                        if (animation)
                            animation->animate("move_up", 1.0f, false,
                                               &unlock_body);
                        body.lock = true;
                    }
                    body.pop_command();
                } break;

                case Command::Type::MOVE_DOWN: {
                    if (this->move(body, 0, -1)) {
                        if (animation)
                            animation->animate("move_down", 1.0f, false,
                                               &unlock_body);
                        body.lock = true;
                    }
                    body.pop_command();
                } break;

                case Command::Type::MOVE_RIGHT: {
                    if (this->move(body, 1, 0)) {
                        if (animation)
                            animation->animate("move_right", 1.0f, false,
                                               &unlock_body);
                        body.lock = true;
                    }
                    body.pop_command();
                } break;

                case Command::Type::MOVE_LEFT: {
                    if (this->move(body, -1, 0)) {
                        if (animation)
                            animation->animate("move_left", 1.0f, false,
                                               &unlock_body);
                        body.lock = true;
                    }
                    body.pop_command();
                } break;

                case Command::Type::GO_TO: {
                    const Vec2i destination = command.data.Cast<Vec2i>();
                    const Vec2i diff = destination - body.position;
                    const Vec2i& pos = body.position;

                    if ((diff.x == 0 && diff.y == 0) ||
                        !m_map(destination).free) {
                        body.pop_command();
                        break;
                    }

                    if (abs(diff.x) > abs(diff.y)) {
                        if (diff.x > 0) {
                            if (m_map(pos.x + 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_RIGHT);
                            } else if (m_map(pos.x, pos.y + 1).free) {
                                body.execute(Command::Type::MOVE_UP);
                            } else if (m_map(pos.x, pos.y - 1).free) {
                                body.execute(Command::Type::MOVE_DOWN);
                            } else if (m_map(pos.x - 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_LEFT);
                            } else {
                                body.pop_command();
                            }

                        } else {
                            if (m_map(pos.x - 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_LEFT);
                            } else if (m_map(pos.x, pos.y + 1).free) {
                                body.execute(Command::Type::MOVE_UP);
                            } else if (m_map(pos.x, pos.y - 1).free) {
                                body.execute(Command::Type::MOVE_DOWN);
                            } else if (m_map(pos.x + 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_RIGHT);
                            } else {
                                body.pop_command();
                            }
                        }

                    } else {
                        if (diff.y > 0) {
                            if (m_map(pos.x, pos.y + 1).free) {
                                body.execute(Command::Type::MOVE_UP);
                            } else if (m_map(pos.x + 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_RIGHT);
                            } else if (m_map(pos.x - 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_LEFT);
                            } else if (m_map(pos.x, pos.y - 1).free) {
                                body.execute(Command::Type::MOVE_DOWN);
                            } else {
                                body.pop_command();
                            }

                        } else {
                            if (m_map(pos.x, pos.y - 1).free) {
                                body.execute(Command::Type::MOVE_DOWN);
                            } else if (m_map(pos.x - 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_LEFT);
                            } else if (m_map(pos.x + 1, pos.y).free) {
                                body.execute(Command::Type::MOVE_RIGHT);
                            } else if (m_map(pos.x, pos.y + 1).free) {
                                body.execute(Command::Type::MOVE_UP);
                            } else {
                                body.pop_command();
                            }
                        }
                    }

                } break;

                default:
                    assert(false, "Unknown command");
                    body.pop_command();
                    break;
            };
        }

        if (body.dirty && body.parent->has_component<PhysicsComponent>()) {
            auto& physics = body.parent->component<PhysicsComponent>();
            physics.position = this->real_position(body.position, physics.size);
            this->set_position(body, body.position.x, body.position.y);
            body.dirty = false;
        }
    }

    for (auto& cursor : cursors) {
        Entity* selected = cursor.selected;
        if (!selected) continue;
        auto& cursor_physics = cursor.parent->component<PhysicsComponent>();
        auto& selected_physics = selected->component<PhysicsComponent>();
        cursor_physics.size = Vec3f(selected_physics.size.x);
        Vec3f relative =
            selected_physics.size * 0.5f - cursor_physics.size * 0.5f;
        cursor_physics.position.x = selected_physics.position.x + relative.x;
        cursor_physics.position.y = selected_physics.position.y + relative.y;
        cursor_physics.position.z = 0.0001f;
    }
}

} /* namespace dm */
