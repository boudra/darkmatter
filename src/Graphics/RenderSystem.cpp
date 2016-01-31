#include "RenderSystem.hpp"

#include "Resource/Texture.hpp"
#include "System/Window.hpp"
#include "Math/Math.hpp"
#include "Resource/Font.hpp"
#include "Resource/ResourceManager.hpp"
#include "Core/Engine.hpp"
#include "Animation/Animation.hpp"

#include "Components/RenderComponent.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/BodyComponent.hpp"
#include "Components/AnimationComponent.hpp"
#include "Components/Cursor.hpp"

#include <GL/glew.h>

#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <SDL2/SDL.h>

#include "Graphics/Camera.hpp"

namespace dm {

RenderSystem::RenderSystem()
    : SystemBase("Render", {SYSTEM(Camera), SYSTEM(AnimationSystem)},
                 {COMPONENT(Render2d)}) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::update() {}

void RenderSystem::component_added(const uint32_t component_type, Entity e) {}

void RenderSystem::render(float interpolation) {
    GLenum error = GL_NO_ERROR;

    m_sprite_batch.set_view_matrix(m_camera->view());

    // const Vec3f& camera_position = m_camera->position();

    m_sprite_batch.begin();

    while ((error = glGetError()) != GL_NO_ERROR) {
        Log::error("OpenGL Error %X (%d)", error, error);
    }

    m_sprite_batch.draw(m_terrain);

    terrain_shader_.bind();
    terrain_shader_.set_uniform(m_camera->view(), "u_view_matrix");

    m_main_shader.bind();
    m_main_shader.set_uniform(m_camera->view(), "u_view_matrix");
    m_projection.ortho({{-m_camera->zoom(), -m_camera->zoom() * 0.5625f},
                        {m_camera->zoom(), m_camera->zoom() * 0.5625f}},
                       {-1.0f, 100.0f});
    m_main_shader.set_uniform(m_projection, "u_projection_matrix");

    for (auto& render : COMPONENTS(Render2d)) {
        if (render.shadow && render.billboard &&
            render.parent->has_component<PhysicsComponent>()) {
            auto& physics = render.parent->component<PhysicsComponent>();
            // physics.rotation.rotate(angle, {1.0f, 0.0f, 0.0f});

            if (!render.has_sprite("shadow")) {
                render.add_sprite(
                    "shadow", Vec3f{0.2f, 0.1f, 0.002f}, Vec2f{0.6f, 0.5f},
                    GET_MANAGER(ResourceManager)->get_texture_id("shadow.png"),
                    Color::White);
            }

            // m_shadow_sprite.position = physics.position;
            // m_shadow_sprite.position.y -= physics.size.x * 0.2f;
            // m_shadow_sprite.position.x -= physics.size.x * 0.1f;
            // m_shadow_sprite.position.z = 0.00001f;

            // // m_shadow_sprite.scale = Vec3f{1.0f} + Vec3f{1.5f} *
            // (physics.position.y * 9.5f);
            // m_shadow_sprite.size = Vec2f{physics.size.x * 1.2f,
            // physics.size.x *
            // 0.5f};
            // Vec2f half = m_shadow_sprite.size * 0.5f;

            // m_shadow_sprite.scale_origin = Vec3f{-half.x, half.y, 0.0f};

            // m_sprite_batch.draw(m_shadow_sprite);
        }
    }

    int cnt = 0;

    for (auto& sprite : MemoryPool<Sprite>::instance()) {
        sprite.shader = &m_main_shader;
        m_sprite_batch.draw(sprite);
        cnt++;
    }

    m_sprite_batch.draw(m_selected->component<Render2d>().get_sprite("cursor"));

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << m_time << " ms"
       << " / " << 1000.0f / (float)m_time << " fps" << std::endl;
    ss << "Camera: " << m_camera->view() << std::endl;
    ss << "Cursor: " << m_cursor->component<BodyComponent>().position
       << std::endl;
    ss << "Sprites: " << MemoryPool<Sprite>::instance().size() << std::endl;
    ss << "Sprites dr: " << cnt << std::endl;

    std::string test = ss.str();

    // m_sprite_batch.draw(Vec2f(20.f, 679.f), "DroidSans 20", ss.str(),
    // Color::Black);
    m_sprite_batch.draw(Vec2f(20.f, 680.f), "Monaco 20", ss.str(),
                        Color::Yellow);

    m_sprite_batch.end();
}

Entity* RenderSystem::create_cursor(const std::string& texture,
                                    const Vec4i& color) {
    Entity* entity = new Entity();

    auto& render = entity->assign<Render2d>();
    auto& physics = entity->assign<PhysicsComponent>();
    auto& anim = entity->assign<AnimationComponent>();
    auto& body = entity->assign<BodyComponent>();

    body.ghost = true;

    entity->assign<Cursor>();

    render.add_sprite("cursor", {0.0f, 0.0f, 0.0f}, Vec2f{1.0f, 1.0f},
                      GET_MANAGER(ResourceManager)->get_texture_id(texture));

    render.get_sprite("cursor").color = color;
    render.shadow = false;
    render.billboard = true;

    // physics.rotation.rotate(-M_PI * 0.5f, {1.0f, 0.0f, 0.0f});
    physics.position.z = 0.0001f;
    physics.set_size(TILE_SIZE);
    physics.scale_origin = physics.size * 0.5f;
    physics.scale_origin.z = 0.0f;
    physics.rotation_origin = Vec3f{0.0f};

    anim.animate("cursor", 1.0f, true);

    return entity;
}

bool RenderSystem::initialize() {
    m_sprite_batch.initialize(GET_MANAGER(ResourceManager));

    m_camera = s_engine->system<Camera>();

    m_main_shader.load("data/shaders/vertex.glsl", dm::Shader::Type::Vertex);
    m_main_shader.load("data/shaders/frag.glsl", dm::Shader::Type::Fragment);
    m_main_shader.link();

    terrain_shader_.load("data/shaders/terrain_vertex.glsl",
                         dm::Shader::Type::Vertex);
    terrain_shader_.load("data/shaders/terrain_frag.glsl",
                         dm::Shader::Type::Fragment);
    terrain_shader_.link();

    m_sprite_batch.set_default_shader(&m_main_shader);

    m_projection.identity();
    // m_projection.perspective(1280.0f / 720.0f, 0.1f, 100.0f, 1.0f);
    m_projection.ortho({{-0.2f, -0.2f * 0.5625f}, Vec2f{0.2f, 0.2f * 0.5625f}},
                       {-1.0f, 100.0f});

    m_ui_ortho.identity();
    m_ui_ortho.ortho({{0.0f, 0.0f}, {1920.0f, 1080.0f}}, {0.1f, 100.0f});

    terrain_shader_.bind();
    terrain_shader_.set_uniform(m_projection, "u_projection_matrix");

    m_main_shader.bind();
    m_main_shader.set_uniform(1, "u_sampler");
    m_main_shader.set_uniform(m_projection, "u_projection_matrix");

    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 16);
    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 18);
    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 20);
    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 24);
    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 30);
    GET_MANAGER(ResourceManager)->load_font("data/visitor.ttf", 34);

    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 16);
    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 18);
    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 20);
    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 24);
    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 30);
    GET_MANAGER(ResourceManager)->load_font("data/DroidSans.ttf", 34);

    m_terrain.crop = Rectangle({0.0f}, {320.0f});
    m_terrain.texture =
        GET_MANAGER(ResourceManager)->load_texture("data/wood.png");
    m_terrain.position =
        Vec3f(-160.0f * TILE_SIZE.x, -160.0f * TILE_SIZE.z, 0.0f);
    m_terrain.size = Vec2f(320.0f * TILE_SIZE.x, 320.0f * TILE_SIZE.z);
    m_terrain.color = Color::White;
    m_terrain.transform.identity();
    m_terrain.shader = &m_main_shader;

    Texture* tile = GET_MANAGER(ResourceManager)->get_texture("wood.png");

    tile->set_wrap_mode(Texture::Wrap::Repeat, Texture::Wrap::Repeat);

    tile->bind();
    tile->setup();
    tile->release();

    m_shadow_sprite.crop = Rectangle({0.0f}, {1.0f});
    m_shadow_sprite.texture =
        GET_MANAGER(ResourceManager)->load_texture("data/shadow.png");
    m_shadow_sprite.position = Vec3f{0.0f, 0.00f, 0.00f};
    m_shadow_sprite.size = Vec2f{0.0f};
    m_shadow_sprite.color = Color::White;
    m_shadow_sprite.shader = &m_main_shader;
    m_shadow_sprite.transform.identity();
    m_shadow_sprite.color = Vec4i{255, 255, 255, 255};

    AnimationSystem* animation = s_engine->system<AnimationSystem>();

    animation->register_animation(
        {"move_left",
         AnimationType::PhysicsPosition,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{300.0f, {-TILE_SIZE.x, 0.0f, 0.0f}},
         }});

    animation->register_animation(
        {"move_up",
         AnimationType::PhysicsPosition,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{300.0f, {0.0f, TILE_SIZE.y, 0.0f}},
         }});

    animation->register_animation(
        {"move_down",
         AnimationType::PhysicsPosition,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{300.0f, {0.0f, -TILE_SIZE.y, 0.0f}},
         }});

    animation->register_animation(
        {"move_right",
         AnimationType::PhysicsPosition,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{300.0f, {TILE_SIZE.x, 0.0f, 0.0f}},
         }});

    animation->register_animation(
        {"cursor",
         AnimationType::PhysicsScale,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{400.0f, {0.1f}},
         }});

    animation->register_animation(
        {"jump",
         AnimationType::PhysicsPosition,
         {
             new AnimationFrame<Vec3f>{0.0f, {0.0f}},
             new AnimationFrame<Vec3f>{100.0f, {0.0f, 0.1f, 0.0f}},
             new AnimationFrame<Vec3f>{200.0f, {0.0f}},
         }});

    animation->register_animation("data/animations/item_idle.json");
    animation->register_animation("data/animations/idle.json");
    animation->register_animation("data/animations/running.json");
    // animation->register_animation("data/animations/running_shadow.json");

    GET_MANAGER(ResourceManager)->load_texture("data/cursor.png");
    // GET_MANAGER(ResourceManager)->load_texture("data/selected.png");

    m_cursor = this->create_cursor("cursor.png", Color::White);
    m_selected = this->create_cursor("cursor.png", Vec4i(63, 166, 239, 255));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    return true;
}

} /* namespace dm */
