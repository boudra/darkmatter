#include "BufferObject.hpp"
#include "Core/Logger.hpp"

#include <GL/glew.h>

namespace dm {

BufferObject::Type buffer_type(const GLenum type) {
    switch (type) {
        case GL_ARRAY_BUFFER:
            return BufferObject::Type::VBO;
        case GL_ELEMENT_ARRAY_BUFFER:
            return BufferObject::Type::IBO;
        default:
            Log::fatal("Unknown buffer type");
    };
}

GLenum buffer_type(const BufferObject::Type type) {
    switch (type) {
        case BufferObject::Type::VBO:
            return GL_ARRAY_BUFFER;
        case BufferObject::Type::IBO:
            return GL_ELEMENT_ARRAY_BUFFER;
        default:
            Log::fatal("Unknown buffer type");
    };
}

BufferObject::BufferObject(unsigned int size, unsigned int element_size,
                           Type type)
    : m_id(0),
      m_capacity(size),
      m_element_size(element_size),
      m_type(buffer_type(type)),
      m_bound(false) {}

BufferObject::~BufferObject() { this->remove(); }
void BufferObject::set_type(const Type type) { m_type = buffer_type(type); }

const BufferObject::Type BufferObject::type() const {
    return buffer_type(m_type);
}

unsigned int BufferObject::create() {
    assert(m_id == 0, "BufferObject musn't be created");
    glGenBuffers(1, &m_id);
    return m_id;
}

void BufferObject::remove() {
    if (m_id != 0) {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

void BufferObject::bind() {
    assert(m_id != 0 && !m_bound, "BufferObject must be created and not bound");
    glBindBuffer(m_type, m_id);
    m_bound = true;
}

void BufferObject::release() {
    glBindBuffer(m_type, 0);
    m_bound = false;
}

void* BufferObject::p_map(const Access access) {
    assert(m_bound, "BufferObject not bound");

    GLenum opengl_access;

    switch (access) {
        case Access::ReadOnly:
            opengl_access = GL_READ_ONLY;
            break;
        case Access::WriteOnly:
            opengl_access = GL_WRITE_ONLY;
            break;
        case Access::ReadWrite:
            opengl_access = GL_READ_WRITE;
            break;
        default:
            Log::fatal("Invalid access policy");
    };

    void* ptr = glMapBuffer(m_type, opengl_access);
    assert(ptr, "Failed to map the buffer, glMapBuffer returned NULL");
    m_mapped = true;

    return ptr;
}

void BufferObject::unmap() {
    assert(m_bound, "BufferObject must be bound");
    glUnmapBuffer(m_type);
    m_mapped = false;
}

void BufferObject::p_set_data(const void* data, const Usage usage) {
    GLenum opengl_usage;

    assert(m_bound, "BufferObject must be bound");

    switch (usage) {
        case Usage::StreamDraw:
            opengl_usage = GL_STREAM_DRAW;
            break;
        case Usage::StreamRead:
            opengl_usage = GL_STREAM_READ;
            break;
        case Usage::StreamCopy:
            opengl_usage = GL_STREAM_COPY;
            break;

        case Usage::StaticDraw:
            opengl_usage = GL_STATIC_DRAW;
            break;
        case Usage::StaticRead:
            opengl_usage = GL_STATIC_READ;
            break;
        case Usage::StaticCopy:
            opengl_usage = GL_STATIC_COPY;
            break;

        case Usage::DynamicDraw:
            opengl_usage = GL_DYNAMIC_DRAW;
            break;
        case Usage::DynamicRead:
            opengl_usage = GL_DYNAMIC_READ;
            break;
        case Usage::DynamicCopy:
            opengl_usage = GL_DYNAMIC_COPY;
            break;

        default:
            assert(false, "Invalid usage flag");
    };

    glBufferData(m_type, m_capacity * m_element_size, data, opengl_usage);
}

} /* namespace dm */
