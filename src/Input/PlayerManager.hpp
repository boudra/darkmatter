#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include "Core/Manager.hpp"
#include "Core/EntityManager.hpp"
#include "Event/Event.hpp"

namespace dm {

  class PlayerManager : public Manager<PlayerManager>
  {
  public:

    PlayerManager();
    ~PlayerManager() {}

    void button_pressed(const KeyboardEvent& e);
    void button_released(const KeyboardEvent& e);
    void mouse_motion(const MouseEvent& e);
    void mouse_click(const MouseEvent& e); 

    void set_player(Entity* player);
    const Entity* player() const;

  private:

    Entity* m_player;

    Vec2i m_selected;
    Vec2i m_hover;

  };

}

#endif
