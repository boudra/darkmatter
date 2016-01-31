#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Core/Component.hpp"

namespace dm {

dm_start_component(Cursor);
Entity *selected = nullptr;
dm_end_component(Cursor);

} /* namespace dm */

#endif /* end of include guard: CURSOR_HPP */
