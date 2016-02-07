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

class RenderSystem : public System {
   public:
    RenderSystem() {}
    ~RenderSystem() {}

    void render(const GameState&);
    void update(GameState&);

   private:
    SpriteBatch _sprite_batch;
};

dm_internal_register_type(RenderSystem)

} /* namespace dm */

#endif /* end of include guard: RENDER_SYSTEM_HPP */
