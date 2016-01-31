#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Core/System.hpp"

#include "Spritebatch.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Font.hpp"
#include "Core/EntityManager.hpp"
#include "Core/ObjectPool.hpp"

namespace dm {

class Camera;

class RenderSystem : public SystemBase {
   public:
    RenderSystem();
    ~RenderSystem();

    void render(float interpolation);
    void update();

    bool initialize();

    void component_added(const uint32_t component_type, Entity e);

    void set_frame_time(uint32_t time) { m_time = time; }

    Entity *create_cursor(const std::string &texture,
                          const Vec4i &color = Color::Yellow);

    Entity *cursor() { return m_cursor; }

    Entity *selected() { return m_selected; }

    const Matrix4f &projection() const { return m_projection; }

    ProgramShader &shader() { return m_main_shader; }

    ProgramShader &terrain_shader() { return terrain_shader_; }

   private:
    SpriteBatch m_sprite_batch;
    Sprite m_terrain;
    Sprite m_shadow_sprite;

    Entity *m_cursor;
    Entity *m_selected;

    uint32_t m_time;
    Matrix4f m_projection;
    Matrix4f m_ui_ortho;
    Camera *m_camera;

    ObjectPool<Sprite> m_sprites;

    ProgramShader m_main_shader;
    ProgramShader terrain_shader_;
};

} /* namespace dm */

#endif /* end of include guard: RENDER_SYSTEM_HPP */
