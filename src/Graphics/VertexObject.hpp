#ifndef VERTEXOBJECT_HPP
#define VERTEXOBJECT_HPP

#include "Graphics/BufferObject.hpp"
#include "Math/Matrix4.hpp"

#include <vector>

namespace dm {

class VertexObject {
   public:
    enum class Primitive {
        Float,
        Integer,
        UnsignedInteger,
        Byte,
        UnsignedByte
    };

    VertexObject();
    ~VertexObject();

    unsigned int create();
    void destroy();

    void bind();
    void release();

    void add_attribute(size_t buffer, unsigned int location, unsigned int count,
                       Primitive valueType, unsigned int stride,
                       unsigned int offset);

    void set_divisor(unsigned int location, unsigned int divisor);

    void release_buffers() {
        for (BufferObject &buffer : m_buffers) buffer.release();
    }

    BufferObject &get_buffer(const size_t index) { return m_buffers[index]; }

    BufferObject &get_bufer_location(const size_t location) {
        return m_buffers[m_locations[location]];
    }

    template <class ValueType>
    size_t add_buffer(const BufferObject::Type type, const unsigned int size) {
        m_buffers.emplace_back(size, sizeof(ValueType), type);
        BufferObject &buffer = m_buffers.back();
        buffer.create();
        return m_buffers.size() - 1;
    }

   private:
    std::vector<BufferObject> m_buffers;
    std::vector<size_t> m_locations;
    unsigned int m_id;
    bool m_bound;
};

} /* namespace dm */

#endif /* end of include guard: VERTEXOBJECT_HPP */
