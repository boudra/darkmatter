#include "PlayerManager.hpp"

#include "Core/EntityManager.hpp"
#include "Core/Engine.hpp"

#include "Components/PlayerComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Components/AnimationComponent.hpp"
#include "Components/Cursor.hpp"

#include "Graphics/Camera.hpp"
#include "Graphics/World.hpp"
#include "Graphics/RenderSystem.hpp"
#include "Animation/Animation.hpp"

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

namespace dm {

PlayerManager::PlayerManager() {
    // s_dispatcher->subscribe(this, &PlayerManager::button_pressed,
    // EventType::KEY_PRESSED);
    s_dispatcher->subscribe("key_up", this, &PlayerManager::button_released);
    s_dispatcher->subscribe("mouse_motion", this, &PlayerManager::mouse_motion);
    s_dispatcher->subscribe("mouse_click", this, &PlayerManager::mouse_click);
}

PlayerManager::~PlayerManager() {
    s_dispatcher->unsubscribe(this);
}

void PlayerManager::mouse_click(const MouseEvent& e) {
    TileMap& map = s_engine->system<World>()->map();
    // AnimationSystem* animation = s_engine->System<AnimationSystem>();

    Entity* m_selection = s_engine->system<RenderSystem>()->selected();
    Cursor& cursor = m_selection->component<Cursor>();

    if (!map(m_hover.x, m_hover.y).free) {
        Entity* selected = map(m_hover.x, m_hover.y).entity;
        if (!selected->has_component<Cursor>()) {
            m_selected = m_hover;
            cursor.selected = selected;
            m_player = cursor.selected;
        }
    }

    if (cursor.selected && map(m_hover).free) {
        auto& body = cursor.selected->component<BodyComponent>();
        auto& render = cursor.selected->component<Render2d>();
        auto& body_sprite = render.get_sprite("body");

        Entity* selected_entity = cursor.selected;

        if ((m_hover - body.position).x < 0 &&
            body.direction == Direction::RIGHT) {
            body_sprite.FlipHorizontal();
            body.direction = Direction::LEFT;
        } else if ((m_hover - body.position).x > 0 &&
                   body.direction == Direction::LEFT) {
            body_sprite.FlipHorizontal();
            body.direction = Direction::RIGHT;
        }

        while (!body.commands.empty()) body.pop_command();

        if (selected_entity->has_component<AnimationComponent>()) {
            auto& animation = selected_entity->component<AnimationComponent>();
            size_t animation_id = animation.animate("running", 1.0f, true);

            body.execute(Command::Type::GO_TO, m_hover, [animation_id,
                                                         selected_entity]() {
                auto& animation =
                    selected_entity->component<AnimationComponent>();
                animation.stop(animation_id);
                animation.animate("idle", 1.0f, true);
            });

        } else {
            body.execute(Command::Type::GO_TO, m_hover);
        }
    }
}

void PlayerManager::mouse_motion(const MouseEvent& e) {
    /*

    static const Vec2f NormalizeWindowCoord {1.0f/1280.0f, 1.0f/720.0f};

    auto& physics = m_player.Component<PhysicsComponent>();
    auto& render = m_player.Component<Render2d>();
    auto& player = m_player.Component<PlayerComponent>();

    SpriteVector& sprites = render.Sprites();
    Sprite& weapon = sprites[2];

    Vec3f mouseWorld = e.Position * NormalizeWindowCoord;
    mouseWorld -= Vec3f {0.5, 0.5} - s_engine->System<Camera>()->GetTarget();

    //const Vec3f weaponPosition = physics.GetPosition() + weapon.m_position +
    weapon.m_rotationOrigin;

    //Vec3f direction = mouseWorld - weaponPosition;
    //direction = direction.Normalize();

    //const float angle = std::atan2(direction.y, direction.x);
    const Vec3f playerCenter = physics.GetPosition() +
    physics.GetRotationOrigin();

    if((mouseWorld.x > playerCenter.x && player.m_direction.x < 0.0f) ||
       (mouseWorld.x < playerCenter.x && player.m_direction.x > 0.0f))
    {
       player.m_direction.x = -player.m_direction.x;
       sprites[BODY].FlipHorizontal();
       sprites[HEAD].FlipHorizontal();
       sprites[WEAPON].FlipVertical();
    }

    //weapon.m_rotation.AngleAxis(angle, {0.0f, 0.0f, 1.0f});

    */

    static const Vec2f normalize_coordinates{1.0f / 1280.0f, 1.0f / 720.0f};

    Entity* cursor = s_engine->system<RenderSystem>()->cursor();

    Matrix4f projection = s_engine->system<RenderSystem>()->projection();
    projection.inverse();

    Matrix4f view = s_engine->system<Camera>()->view();
    view.inverse();

    Vec3f mouse_world = (e.position * normalize_coordinates) * 2.0f - 1.0f;
    mouse_world.z = 0.0f;

    // LOG(DEBUG, "MOUSE NORMALIZED: " << mouse_world);

    Vec3f eye = projection * mouse_world;

    Vec4f world{eye.x, eye.y, -1.0f, 0.0f};
    world = view * world;
    world.normalize();

    Vec3f ray{world.x, world.y, world.z};
    Vec3f start = s_engine->system<Camera>()->position();

    // LOG(DEBUG, "RAY START     -> " << start);
    // LOG(DEBUG, "RAY DIRECTION -> " << ray);

    Vec3f plane_normal{0.0f, 0.0f, -1.0f};

    float t = -(plane_normal.dot(start) / plane_normal.dot(ray));

    // LOG(DEBUG, "T -> " << t);

    Vec3f point = start + (ray * t);

    // LOG(DEBUG, "POINT -> " << point);

    auto& body = cursor->component<BodyComponent>();
    auto& physics = cursor->component<PhysicsComponent>();

    Vec2i tile{int(std::floor(point.x / TILE_SIZE.x)),
               int(std::floor(point.y / TILE_SIZE.y))};

    body.position = tile;
    m_hover = tile;

    // LOG(DEBUG, "HOVER -> " << point);

    physics.position =
        s_engine->system<World>()->real_position(m_hover, physics.size);
}

void PlayerManager::button_released(const KeyboardEvent& e) {
    switch (e.key) {
        case 'w':
        case 's':
            break;

        case 'a':
        case 'd':
            break;

        case 'p': {
            GLint mode = 0;
            glGetIntegerv(GL_POLYGON_MODE, &mode);
            glPolygonMode(GL_FRONT_AND_BACK,
                          mode == GL_LINE ? GL_FILL : GL_LINE);
            break;
        }

        default:
            break;
    }
}

void PlayerManager::button_pressed(const KeyboardEvent& e) {
    auto& body = m_player->component<BodyComponent>();

    switch (e.key) {
        case 'w':
            body.execute(Command::Type::MOVE_UP);
            break;

        case 's':
            body.execute(Command::Type::MOVE_DOWN);
            break;

        case 'a':
            body.execute(Command::Type::MOVE_LEFT);
            break;

        case 'd':
            body.execute(Command::Type::MOVE_RIGHT);
            break;

        default:
            break;
    }
}

void PlayerManager::set_player(Entity* player) {
    s_engine->system<RenderSystem>()->selected()->component<Cursor>().selected =
        player;
    m_player = player;
}

const Entity* PlayerManager::player() const { return m_player; }
}
