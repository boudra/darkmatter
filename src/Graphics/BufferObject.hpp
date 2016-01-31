#ifndef BUFFEROBJECT_HPP
#define BUFFEROBJECT_HPP

#include <cassert>
#include <algorithm>

namespace dm {

class BufferObject {
   public:
    enum class Type { VBO, IBO };

    enum class Access { ReadOnly, WriteOnly, ReadWrite };

    enum class Usage {
        StreamDraw,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
    };

    BufferObject() : BufferObject(0, 0, Type::VBO) {}
    BufferObject(unsigned int size, unsigned int elementSize, Type type);

    BufferObject(const BufferObject& other) = delete;

    BufferObject(BufferObject&& other) : BufferObject() { swap(other); }

    ~BufferObject();

    void swap(BufferObject& other) {
        std::swap(m_id, other.m_id);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_element_size, other.m_element_size);
        std::swap(m_type, other.m_type);
    }

    BufferObject& operator=(BufferObject&& rhs) {
        swap(rhs);
        BufferObject().swap(rhs);
        return *this;
    }

    const BufferObject& operator=(const BufferObject& rhs) = delete;

    unsigned int create();
    void remove();

    void bind();
    void release();

    const bool operator==(const BufferObject& rhs) { return m_id == rhs.m_id; }

    template <class ValueType>
    void set_data(const ValueType* data, const Usage usage) {
        assert(sizeof(ValueType) == m_element_size);
        p_set_data(static_cast<const void*>(data), usage);
    }

    void resize(unsigned int size) { m_capacity = size; }

    template <class ValueType>
    ValueType* map(const Access access) {
        return static_cast<ValueType*>(p_map(access));
    }

    void unmap();

    void set_type(const Type type);
    const Type type() const;

    const unsigned int id() const { return m_id; }
    const unsigned int element_size() { return m_element_size; }

    template <class ValueType>
    void set_element() {
        m_element_size = sizeof(ValueType);
    }

    const unsigned int capacity() { return m_capacity; }

   private:
    void p_set_data(const void* data, const Usage usage);
    void* p_map(const Access access);

    unsigned int m_id;
    unsigned int m_capacity;
    unsigned int m_element_size;

    unsigned int m_type;
    bool m_mapped;
    bool m_bound;
};

} /* namespace dm */

#endif /* end of include guard: BUFFEROBJECT_HPP */
