#ifndef SPRITEBATCH_HPP
#define SPRITEBATCH_HPP

#include <map>
#include <vector>

#include "Math/Rectangle.hpp"
#include "Math/Vector4.hpp"

#include "Components/RenderComponent.hpp"
#include "Resource/Font.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/BufferObject.hpp"
#include "Graphics/VertexObject.hpp"

namespace dm {

class Texture;
class ResourceManager;

class SpriteBatch {
   public:
    SpriteBatch();
    ~SpriteBatch();

    void begin();
    void end();

    bool initialize(ResourceManager *resource_manager);

    void draw(const Vec2f &position, const Vec2f &size, const size_t &texture,
              const Vec4i &color = Color::White);

    void draw(const Vec3f &position, const Vec2f &size, const size_t &texture,
              const Matrix4f &transform, const Vec4i &color = Color::White);

    void draw(const Rectangle &rectangle, const size_t &sprite,
              const Vec4i &color = Color::White);

    void draw(const Vec2f &position, const std::string &font_name,
              const std::string &text, const Vec4i &color = Color::White);

    void draw(const Matrix4f &transform, const Vec2f &size,
              const size_t &sprite, const Vec4i &color = Color::White);

    void draw(const Sprite &sprite);

    void set_view_matrix(const Matrix4f &matrix) { m_view = matrix; }

    void set_default_shader(ProgramShader *shader) { m_shader = shader; }

   private:
    Sprite &add_sprite(const Sprite &sprite);

    void fill_indices();
    void batch(const size_t start, const size_t end);
    void flush();

    struct Vertex {
        Vec3f position;
        Vec4f color;
        Vec2f texture;
    };

    std::vector<Sprite *> m_ordered_sprites;
    std::vector<Sprite> m_sprites;
    std::vector<Matrix4f> m_transforms;

    size_t m_sprite_count;

    ProgramShader *m_shader;
    ProgramShader m_font_shader;

    BufferObject *m_vertices;
    BufferObject *m_ibo;

    VertexObject m_vao;

    const size_t MinSprites = 128;
    const size_t MaxSprites = 2048;

    Matrix4f m_perspective;
    Matrix4f m_ortho;
    Matrix4f m_view;

    ResourceManager *m_resource_manager;
};

} /* game */

#endif /* end of include guard: m_SPRITEBATCH_HPP_ */
