#ifndef EVENT_HPP
#define EVENT_HPP

#include <Math/Vector2.hpp>

namespace dm {

enum EventType {
    UNKNOWN,
    QUIT,

    /* Components */
    COMPONENT_ADDED,
    COMPONENT_REMOVED,

    /* Entities */
    ENTITY_ADDED,
    ENTITY_REMOVED,

    /* Keyboard Events */
    KEYBOARD,  // General keyboard event
    KEY_PRESSED,
    KEY_RELEASED,

    /* Mouse Events */
    MOUSE_MOTION,
    MOUSE_WHEEL,
    MOUSE_LPRESS,
    MOUSE_RPRESS,

    NUM_EVENTS
};

struct Event {
    Event(EventType type = UNKNOWN) : type(type) {}
    EventType type;
    virtual ~Event() {}
};

struct QuitEvent : Event {
    QuitEvent() : Event(EventType::QUIT) {}
};

struct KeyboardEvent : Event {
    KeyboardEvent(unsigned int key, bool state)
        : Event(state ? EventType::KEY_PRESSED : EventType::KEY_RELEASED),
          state(state),
          key(key) {}

    bool state;
    unsigned int key;
};

struct MouseEvent : Event {
    MouseEvent(const Vec2f& position,
               const EventType type = EventType::MOUSE_MOTION)
        : Event(type), position(position) {}
    Vec2f position;
};

struct MouseWheelEvent : Event {
    MouseWheelEvent(const Vec2i& delta,
                    const EventType type = EventType::MOUSE_WHEEL)
        : Event(type), delta(delta) {}
    Vec2i delta;
};

} /* dm */

#endif
