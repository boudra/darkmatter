#include "Graphics/Spritebatch.hpp"
#include "Resource/Texture.hpp"
#include "Resource/ResourceManager.hpp"

#include "Math/Quaternion.hpp"
#include "Core/Logger.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

namespace dm {

SpriteBatch::SpriteBatch() {}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::fill_indices() {
    std::vector<unsigned short> indices;
    indices.reserve(MaxSprites * 6);

    for (size_t i = 0; i < MaxSprites * 4; i += 4) {
        indices.push_back(i + 0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i + 0);
    }

    m_ibo->set_data<unsigned short>(&indices[0],
                                    BufferObject::Usage::StaticDraw);
}

bool SpriteBatch::initialize(ResourceManager* resource_manager) {
    m_resource_manager = resource_manager;

    m_ordered_sprites.reserve(MinSprites);
    m_sprites.reserve(MinSprites);

    m_ortho.ortho({{0.0f, 0.0f}, {1280.0f, 720.0f}}, {-1.0f, 100.0f});

    m_font_shader.load("data/shaders/font_vertex.glsl",
                       dm::Shader::Type::Vertex);
    m_font_shader.load("data/shaders/font_frag.glsl",
                       dm::Shader::Type::Fragment);
    m_font_shader.link();

    m_font_shader.bind();
    m_font_shader.set_uniform(1, "u_sampler");
    m_font_shader.set_uniform(m_ortho, "u_projection_matrix");

    m_vao.create();

    /* Create the VBO */
    size_t ibo = m_vao.add_buffer<unsigned short>(BufferObject::Type::IBO,
                                                  MaxSprites * 6);
    size_t vbo =
        m_vao.add_buffer<Vertex>(BufferObject::Type::VBO, MinSprites * 4);

    m_ibo = &m_vao.get_buffer(ibo);
    m_vertices = &m_vao.get_buffer(vbo);

    m_vao.bind();
    m_ibo->bind();

    this->fill_indices();

    m_vertices->bind();

    m_vao.add_attribute(vbo, 0, 3, VertexObject::Primitive::Float,
                        sizeof(Vertex), 0);
    m_vao.add_attribute(vbo, 1, 4, VertexObject::Primitive::Float,
                        sizeof(Vertex), sizeof(Vec3f));
    m_vao.add_attribute(vbo, 2, 2, VertexObject::Primitive::Float,
                        sizeof(Vertex), sizeof(Vec3f) + sizeof(Vec4f));
    m_vao.release();
    m_vao.release_buffers();

    return true;
}

void SpriteBatch::begin() {
    m_sprites.clear();
    m_ordered_sprites.clear();
    m_sprite_count = 0;
}

Sprite& SpriteBatch::add_sprite(const Sprite& sprite) {
    m_sprites.push_back(sprite);
    Sprite& s = m_sprites.back();
    if (s.shader == nullptr) s.shader = m_shader;
    m_sprite_count++;

    if (m_sprite_count == MaxSprites) {
        this->flush();
    }

    return s;
}

void SpriteBatch::draw(const Sprite& sprite) { this->add_sprite(sprite); }

void SpriteBatch::draw(const Vec2f& position, const Vec2f& size,
                       const size_t& sprite, const Vec4i& color) {
    this->draw(Rectangle(position, size), sprite, color);
}

void SpriteBatch::draw(const Vec3f& position, const Vec2f& size,
                       const size_t& texture, const Matrix4f& transform,
                       const Vec4i& color) {
    Sprite sprite;

    sprite.transform = transform;
    sprite.color = color;
    sprite.crop = Rectangle(Vec2f(0.0f), Vec2f(1.0f));
    sprite.texture = texture;
    sprite.position = position;
    sprite.size = size;

    this->add_sprite(sprite);
}

void SpriteBatch::draw(const Rectangle& rectangle, const size_t& texture,
                       const Vec4i& color) {
    Sprite sprite;

    sprite.color = color;
    sprite.crop = Rectangle(Vec2f(0.0f), Vec2f(1.f, 1.f));
    sprite.texture = texture;

    this->add_sprite(sprite);
}

void SpriteBatch::draw(const Matrix4f& transform, const Vec2f& size,
                       const size_t& texture, const Vec4i& color) {
    Sprite sprite;

    sprite.transform = transform;
    sprite.color = color;
    sprite.crop = Rectangle(Vec2f(0.0f), Vec2f(1.0f));
    sprite.texture = texture;

    this->add_sprite(sprite);
}

void SpriteBatch::draw(const Vec2f& position, const std::string& font_name,
                       const std::string& text, const Vec4i& color) {
    Vec2f cursor = position;
    Font* font = m_resource_manager->get_font(font_name);

    Sprite sprite;

    sprite.transform.identity();

    for (const char& c : text) {
        const Glyph& g = font->glyph((uint32_t)c);

        sprite.position = cursor + g.vertices.position;
        sprite.size = g.vertices.size;
        sprite.crop = g.crop;
        sprite.color = color;
        sprite.texture = font->texture();
        sprite.shader = &m_font_shader;

        cursor += g.kerning;

        if (c == '\n') {
            cursor.y -= (float)font->size() * 1.5f;
            cursor.x = position.x;
        }

        this->add_sprite(sprite);
    }
}

void SpriteBatch::end() { this->flush(); }

void SpriteBatch::flush() {
    m_ordered_sprites.reserve(m_sprites.size());
    for (Sprite& s : m_sprites) m_ordered_sprites.push_back(&s);

    /* Order by transparency, shader and textures */
    std::sort(m_ordered_sprites.begin(), m_ordered_sprites.end(),
              [this](const Sprite* a, const Sprite* b) {

                  const Texture* tex_a =
                      this->m_resource_manager->get_texture(a->texture);
                  const Texture* tex_b =
                      this->m_resource_manager->get_texture(b->texture);

                  if (tex_a->transparent() && tex_b->transparent())
                      if (a->texture == b->texture)
                          return a->shader > b->shader;
                      else
                          return a->texture > b->texture;
                  else
                      return tex_b->transparent();

              }

              );

    m_vertices->bind();
    m_vertices->resize(std::max(MinSprites * 4, m_sprite_count * 4));
    m_vertices->set_data<Vertex>(nullptr, BufferObject::Usage::StreamDraw);

    Vertex* vertices = m_vertices->map<Vertex>(BufferObject::Access::ReadWrite);

    assert(vertices != nullptr);

    Matrix4f transform, rotation;

    for (size_t i = 0; i < m_sprite_count; ++i) {
        Vertex v[4];

        Sprite& s = *m_ordered_sprites[i];
        const Texture* tex = this->m_resource_manager->get_texture(s.texture);

        v[0].position = s.position;
        v[1].position = s.position;
        v[2].position = s.position;
        v[3].position = s.position;

        v[0].texture = s.crop.position;
        v[1].texture = s.crop.position;
        v[2].texture = s.crop.position;
        v[3].texture = s.crop.position;

        v[0].color = (s.color / 255.0f);

        v[1].position.x += s.size.x;
        v[1].color = (s.color / 255.0f);

        v[2].position += s.size;
        v[2].color = (s.color / 255.0f);

        v[3].position.y += s.size.y;
        v[3].color = (s.color / 255.0f);

        v[1].texture.x += s.crop.size.x;
        v[2].texture += s.crop.size;
        v[3].texture.y += s.crop.size.y;

        transform.identity();

        transform.translate((s.scale_origin));
        transform.scale(s.scale);
        transform.translate(-(s.scale_origin));

        transform.translate((s.rotation_origin));
        s.rotation.get_matrix(rotation);
        transform = transform * rotation;
        transform.translate(-(s.rotation_origin));

        transform = s.transform * transform;

        v[0].position = transform * v[0].position;
        v[1].position = transform * v[1].position;
        v[2].position = transform * v[2].position;
        v[3].position = transform * v[3].position;

        std::memcpy(vertices + (i * 4), &v[0], sizeof(v));
    }

    m_vertices->unmap();
    m_vertices->release();

    glActiveTexture(GL_TEXTURE0 + 1);

    m_vao.bind();

    size_t batch_size = 0, current = 0;
    ProgramShader* last_shader = m_ordered_sprites[0]->shader;

    while (current < m_sprite_count) {
        if (m_ordered_sprites[current]->shader != last_shader) {
            last_shader->bind();

            this->batch(current - batch_size, current);

            last_shader = m_ordered_sprites[current]->shader;
            batch_size = 0;
        }

        batch_size++;
        current++;
    }

    last_shader->bind();

    this->batch(m_sprite_count - batch_size, m_sprite_count);

    m_vao.release();
    m_vao.release_buffers();

    m_sprites.clear();
    m_ordered_sprites.clear();
    m_sprite_count = 0;
}

void SpriteBatch::batch(const size_t start, const size_t end) {
    size_t batch_size = 0, current = start;
    size_t last_texture = m_ordered_sprites[current]->texture;

    while (current < end) {
        if (m_ordered_sprites[current]->texture != last_texture) {
            m_resource_manager->get_texture(last_texture)->bind();

            glDrawElements(GL_TRIANGLES, batch_size * 6, GL_UNSIGNED_SHORT,
                           (void*)(uintptr_t)((current - batch_size) * 6 *
                                              sizeof(unsigned short)));

            batch_size = 0;

            m_resource_manager->get_texture(last_texture)->release();

            last_texture = m_ordered_sprites[current]->texture;
        }

        batch_size++;
        current++;
    }

    m_resource_manager->get_texture(last_texture)->bind();

    glDrawElements(GL_TRIANGLES, batch_size * 6, GL_UNSIGNED_SHORT,
                   (void*)(uintptr_t)((current - batch_size) * 6 *
                                      sizeof(unsigned short)));

    m_resource_manager->get_texture(last_texture)->release();
}

} /* game */
