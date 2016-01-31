#ifndef WORLD_HPP
#define WORLD_HPP

#include "Core/System.hpp"
#include "Components/BodyComponent.hpp"
#include "Math/Vector4.hpp"
#include "Graphics/VertexObject.hpp"

namespace dm {

class TileMap {
   public:
    TileMap() : TileMap(50, 50) {}

    TileMap(const int& width, const int& height)
        : m_width(width), m_height(height) {
        m_tiles.resize(width * height * 2);
    }

    ~TileMap() {}

    Tile& operator()(const int& x, const int& y) {
        return m_tiles[(x + m_width) + ((y + m_height) * m_width)];
    }

    Tile& operator()(const Vec2i position) {
        return (*this)(position.x, position.y);
    }

   private:
    int m_width;
    int m_height;

    std::vector<Tile> m_tiles;
};

class World : public SystemBase {
   public:
    World()
        : SystemBase("World", {}, {COMPONENT(BodyComponent)}),
          m_map(320, 320) {}
    ~World() {}

    bool initialize();

    void update();
    void render(float interpolation);

    bool move(BodyComponent& body, int x, int y);
    void set_position(BodyComponent& body, int x, int y);

    Vec3f real_position(Vec2i position, Vec3f size);
    void component_added(const uint32_t component_type, Entity e);

    TileMap& map() { return m_map; }

    struct Vertex {
        Vec3f position;
        Vec4f color;
    };

    struct Mesh {
        std::vector<Vec3f> vertices;
        std::vector<Vec4f> colors;
        std::vector<Vec3f> normals;
        std::vector<unsigned int> indices;
    };

   private:
    TileMap m_map;

    // BufferObject* vertices_;
    // BufferObject* indices_;
    VertexObject terrain_;

    Mesh terrain_mesh_;
};

} /* namespace dm */

#endif /* end of include guard: WORLD_HPP */
