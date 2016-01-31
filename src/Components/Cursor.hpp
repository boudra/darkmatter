#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Core/Component.hpp"

namespace dm {

struct Cursor : public Component {
    Entity* selected = nullptr;
    dm_register_component(Cursor);
};

} /* namespace dm */

#endif /* end of include guard: CURSOR_HPP */
