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

void RenderSystem::update(GameState& state) {
    if (state.is("initial")) {
        _sprite_batch.initialize(
            &state.get<ResourceManager>("resource_manager"));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }
}

void RenderSystem::render(const GameState& state) {
    if (!state.is("initial")) {
        GLenum error = GL_NO_ERROR;

        _sprite_batch.set_view_matrix(state.get<Matrix4f>("view_matrix"));
        _sprite_batch.set_projection_matrix(
            state.get<Matrix4f>("projection_matrix"));

        _sprite_batch.begin();

        while ((error = glGetError()) != GL_NO_ERROR) {
            Log::error("OpenGL Error %X (%d)", error, error);
        }

        int cnt = 0;

        for (auto& sprite : MemoryPool<Sprite>::instance()) {
            Log::debug("SPRITE?");
            _sprite_batch.draw(sprite);
            cnt++;
        }

        Log::debug("%lu", MemoryPool<Sprite>::instance().size());

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2)
           << state.get<double>("frame_time") << " ms"
           << " / " << 1000.0 / state.get<double>("frame_time") << " fps"
           << std::endl;
        ss << "Sprites dr: " << cnt << std::endl;

        std::string test = ss.str();

        _sprite_batch.draw(Vec2f(20.f, 680.f), "Droid Sans 34", ss.str(),
                           Color::Yellow);
        _sprite_batch.end();
    }
}

} /* namespace dm */
