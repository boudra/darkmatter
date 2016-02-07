#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include "Core/Manager.hpp"
#include "Core/GameState.hpp"
#include "Core/EntityManager.hpp"
#include "Event/Event.hpp"

namespace dm {

class PlayerManager : public Manager<PlayerManager> {
   public:
    PlayerManager();
    ~PlayerManager();

    void button_pressed(const KeyboardEvent& e);
    void button_released(const KeyboardEvent& e);
    void mouse_motion(const MouseEvent& e);
    void mouse_click(const MouseEvent& e);

    const bool initialize(GameState& state) {
        state.subscribe("key_up", this, &PlayerManager::button_released);
        state.subscribe("mouse_motion", this, &PlayerManager::mouse_motion);
        state.subscribe("mouse_click", this, &PlayerManager::mouse_click);
        return true;
    }

    void set_player(Entity* player);
    const Entity* player() const;

   private:
    Entity* m_player;

    Vec2i m_selected;
    Vec2i m_hover;
};
dm_internal_register_type(PlayerManager);
}

#endif
