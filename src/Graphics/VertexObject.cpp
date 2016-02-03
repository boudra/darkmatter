#include "Graphics/VertexObject.hpp"
#include "Core/Logger.hpp"

#include <GL/glew.h>

namespace dm {

GLenum primitive_type(const VertexObject::Primitive type) {
    switch (type) {
        case VertexObject::Primitive::Float:
            return GL_FLOAT;
        case VertexObject::Primitive::Integer:
            return GL_INT;
        case VertexObject::Primitive::UnsignedInteger:
            return GL_UNSIGNED_INT;
        case VertexObject::Primitive::Byte:
            return GL_BYTE;
        case VertexObject::Primitive::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        default:
            assert(false, "Unknown primitive type");
    }
}

VertexObject::VertexObject() : m_id(0), m_bound(false) {}

VertexObject::~VertexObject() {
    this->bind();
    this->destroy();
    this->release();
}

unsigned int VertexObject::create() {
    assert(m_id == 0, "VertexObject already created");
    glGenVertexArrays(1, &m_id);
    return m_id;
}

void VertexObject::destroy() {
    glDeleteVertexArrays(1, &m_id);
    m_id = 0;
}

void VertexObject::bind() {
    assert(!m_bound, "VertexObject already bound");
    glBindVertexArray(m_id);
    m_bound = true;
}

void VertexObject::release() {
    assert(m_bound, "VertexObject not bound");
    glBindVertexArray(0);
    m_bound = false;
}

void VertexObject::add_attribute(size_t buffer, unsigned int location,
                                 unsigned int count, Primitive valueType,
                                 unsigned int stride, unsigned int offset) {
    assert(m_buffers.size() > buffer && m_bound, "Unknown buffer");

    glVertexAttribPointer(location, count, primitive_type(valueType), GL_FALSE,
                          stride, (GLvoid*)(uintptr_t)offset);
    glEnableVertexAttribArray(location);

    if (m_locations.size() <= location) {
        m_locations.resize(location + 1);
    }

    m_locations[location] = buffer;
}

void VertexObject::set_divisor(unsigned int location, unsigned int divisor) {
    assert(m_bound, "VertexObject not bound");
    glVertexAttribDivisor(location, 1);
}

} /* namespace dm */
