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

void debug_mesh(World::Mesh &mesh) {
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        const unsigned int &index = mesh.indices[i];
        const unsigned int &index1 = mesh.indices[i + 1];
        const unsigned int &index2 = mesh.indices[i + 2];
    }
}

bool find_common_side(const uint32_t a[3], const uint32_t b[3],
                      uint32_t result[2][2]) {
    uint32_t next = 0;
    size_t i = 0;

    for (i = 0; i < 3 && next < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (a[i] == b[j]) {
                result[next][0] = i;
                result[next][1] = j;
                next++;
                break;
            }
        }
    }

    assert(next <= 2);

    return (next == 2);
}

void triangulate(World::Mesh &mesh) {
    std::vector<uint32_t> indices;
    std::vector<uint32_t> done;

    uint32_t common_side[2][2] = {{0}};

    indices.reserve(mesh.indices.size() * 2);
    done.reserve(mesh.indices.size());

    mesh.vertices.reserve(mesh.vertices.size() + (mesh.vertices.size() / 3));
    mesh.normals.reserve(mesh.normals.size() + (mesh.normals.size() / 3));

    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        for (size_t j = i + 3; j < mesh.indices.size(); j += 3) {
            if (std::find(done.begin(), done.end(), j) != done.end()) continue;
            if (std::find(done.begin(), done.end(), i) != done.end()) continue;

            if (find_common_side(&mesh.indices[i], &mesh.indices[j],
                                 common_side)) {
                const Vec3f pivot =
                    (mesh.vertices[mesh.indices[i + common_side[0][0]]] +
                     mesh.vertices[mesh.indices[i + common_side[1][0]]]) *
                    0.5f;

                const uint32_t outer_index_a =
                    mesh.indices[i + 3 -
                                 (common_side[0][0] + common_side[1][0])];
                const uint32_t outer_index_b =
                    mesh.indices[j + 3 -
                                 (common_side[0][1] + common_side[1][1])];

                const uint32_t pivot_index = mesh.vertices.size();
                mesh.vertices.push_back(pivot);

                Vec3f centroid{0.0f, 0.0f, 1.0f};

                centroid.x +=
                    (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.05f;
                centroid.y +=
                    (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.05f;

                centroid.normalize();

                mesh.normals.push_back(centroid);
                mesh.colors.push_back(mesh.colors[mesh.indices[i]]);

                indices.push_back(pivot_index);
                indices.push_back(outer_index_a);
                indices.push_back(mesh.indices[i + common_side[1][0]]);

                indices.push_back(pivot_index);
                indices.push_back(mesh.indices[i + common_side[0][0]]);
                indices.push_back(outer_index_a);

                indices.push_back(pivot_index);
                indices.push_back(outer_index_b);
                indices.push_back(mesh.indices[i + common_side[1][1]]);

                indices.push_back(pivot_index);
                indices.push_back(mesh.indices[i + common_side[0][1]]);
                indices.push_back(outer_index_b);

            } else {
            }

            done.push_back(i);
            done.push_back(j);
        }
    }

    mesh.indices = std::move(indices);
}

bool World::initialize() {
    const Vec3f position{-2.0f, -2.0f, 0.0f};
    const Vec3f size{5.0f, 5.0f, 0.0f};
    const Vec4f color{0.4313f, 0.30980f, 0.2f, 1.0f};

    terrain_mesh_.vertices.push_back(position);
    terrain_mesh_.vertices.push_back(position);
    terrain_mesh_.vertices.push_back(position + size);
    terrain_mesh_.vertices.push_back(position);

    terrain_mesh_.vertices[1].x += size.x;
    terrain_mesh_.vertices[3].y += size.y;

    terrain_mesh_.colors.push_back(color);
    terrain_mesh_.colors.push_back(color);
    terrain_mesh_.colors.push_back(color);
    terrain_mesh_.colors.push_back(color);

    terrain_mesh_.normals.push_back({0.0f, 0.0f, 1.0f});
    terrain_mesh_.normals.push_back({0.0f, 0.0f, 1.0f});
    terrain_mesh_.normals.push_back({0.0f, 0.0f, 1.0f});
    terrain_mesh_.normals.push_back({0.0f, 0.0f, 1.0f});

    terrain_mesh_.indices.push_back(0);
    terrain_mesh_.indices.push_back(1);
    terrain_mesh_.indices.push_back(2);
    terrain_mesh_.indices.push_back(2);
    terrain_mesh_.indices.push_back(3);
    terrain_mesh_.indices.push_back(0);

    debug_mesh(terrain_mesh_);

    for (size_t i = 0; i < 2; i++) {
        triangulate(terrain_mesh_);
    }

    terrain_.create();

    size_t vbo_verts = terrain_.add_buffer<Vec3f>(
        BufferObject::Type::VBO, terrain_mesh_.vertices.size());
    size_t vbo_colors = terrain_.add_buffer<Vec4f>(BufferObject::Type::VBO,
                                                   terrain_mesh_.colors.size());
    size_t vbo_normals = terrain_.add_buffer<Vec3f>(
        BufferObject::Type::VBO, terrain_mesh_.normals.size());
    size_t ibo = terrain_.add_buffer<unsigned int>(
        BufferObject::Type::IBO, terrain_mesh_.indices.size());

    terrain_.bind();

    terrain_.get_buffer(ibo).bind();
    terrain_.get_buffer(ibo).set_data<unsigned int>(
        &terrain_mesh_.indices[0], BufferObject::Usage::StaticDraw);

    terrain_.get_buffer(vbo_verts).bind();
    terrain_.add_attribute(vbo_verts, 0, 3, VertexObject::Primitive::Float,
                           sizeof(Vec3f), 0);
    terrain_.get_buffer(vbo_verts).set_data<Vec3f>(
        &terrain_mesh_.vertices[0], BufferObject::Usage::StaticDraw);

    terrain_.get_buffer(vbo_colors).bind();
    terrain_.add_attribute(vbo_colors, 1, 4, VertexObject::Primitive::Float,
                           sizeof(Vec4f), 0);
    terrain_.get_buffer(vbo_colors)
        .set_data<Vec4f>(&terrain_mesh_.colors[0],
                         BufferObject::Usage::StaticDraw);

    terrain_.get_buffer(vbo_normals).bind();
    terrain_.add_attribute(vbo_normals, 2, 3, VertexObject::Primitive::Float,
                           sizeof(Vec3f), 0);
    terrain_.get_buffer(vbo_normals)
        .set_data<Vec3f>(&terrain_mesh_.normals[0],
                         BufferObject::Usage::StaticDraw);

    terrain_.release();
    terrain_.release_buffers();

    return true;
}

void unlock_body(Entity *e) { e->component<BodyComponent>().lock = false; }

bool World::move(BodyComponent &body, int x, int y) {
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

void World::set_position(BodyComponent &body, int x, int y) {
    if (!body.ghost) {
        Tile &last_position = m_map(body.position);
        Tile &new_position = m_map(x, y);

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

void World::update() {
    auto &bodies = COMPONENTS(BodyComponent);
    auto &cursors = COMPONENTS(Cursor);

    for (auto &body : bodies) {
        Entity *e = body.parent;
        AnimationComponent *animation =
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
                    const Vec2i &pos = body.position;

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
                    assert(false);
                    body.pop_command();
                    break;
            };
        }

        if (body.dirty && body.parent->has_component<PhysicsComponent>()) {
            auto &physics = body.parent->component<PhysicsComponent>();
            physics.position = this->real_position(body.position, physics.size);
            this->set_position(body, body.position.x, body.position.y);
            body.dirty = false;
        }
    }

    for (auto &cursor : cursors) {
        Entity *selected = cursor.selected;
        if (!selected) continue;
        auto &cursor_physics = cursor.parent->component<PhysicsComponent>();
        auto &selected_physics = selected->component<PhysicsComponent>();
        cursor_physics.size = Vec3f(selected_physics.size.x);
        Vec3f relative =
            selected_physics.size * 0.5f - cursor_physics.size * 0.5f;
        cursor_physics.position.x = selected_physics.position.x + relative.x;
        cursor_physics.position.y = selected_physics.position.y + relative.y;
        cursor_physics.position.z = 0.0001f;
    }
}

void World::render(float interpolation) {}

} /* namespace dm */
