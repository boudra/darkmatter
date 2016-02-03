#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <cstdint>
#include <vector>
#include <algorithm>
#include <sys/types.h>

namespace dm {

template <typename value_type>
class ObjectPool {
   public:
    ObjectPool(const size_t chunk_size = 1024) : m_chunk_size(chunk_size) {}
    ~ObjectPool() {}

    size_t create() {
        ssize_t id = this->find_free();

        if (id > -1) {
            m_free[id] = false;
        } else {
            m_free.push_back(false);
            m_objects.emplace_back();
            id = m_objects.size() - 1;
        }

        return id;
    }

    size_t push(const value_type& value) {
        ssize_t id = this->find_free();

        if (id > -1) {
            m_free[id] = false;
            m_objects[id] = value;
        } else {
            m_objects.emplace_back(value);
            m_free.push_back(false);
        }

        return id;
    }

    ssize_t find_free() {
        auto free = std::find(m_free.begin(), m_free.end(), true);
        ssize_t id = -1;

        if (free != m_free.end()) {
            id = free - m_free.begin();
        }

        return id;
    }

    void remove(const size_t& id) {
        m_free[id] = true;
        m_objects[id] = value_type();
    }

    value_type& operator[](const size_t& id) {
        assert(id >= 0 && id < m_objects.size(), "Index out of bounds");
        return m_objects[id];
    }

    value_type& get(const size_t& id) { return (*this)[id]; }

    class iterator : public std::iterator<std::input_iterator_tag, value_type> {
       private:
        const std::vector<bool>& m_free;
        std::vector<value_type>& m_components;
        size_t m_index;

        void next() {
            const size_t m_free_size = m_free.size();
            ++m_index;
            while (m_index < m_free_size && m_free[m_index] == true) ++m_index;
        }

       public:
        iterator(const std::vector<bool>& free,
                 std::vector<value_type>& components, size_t index)
            : m_free(free), m_components(components), m_index(index) {}

        iterator& operator++() {
            this->next();
            return *this;
        }

        bool operator==(const iterator& rhs) const {
            return m_index == rhs.m_index;
        }

        bool operator!=(const iterator& rhs) const {
            return m_index != rhs.m_index;
        }

        value_type& operator*() { return m_components[m_index]; }
        value_type* operator->() { return &m_components[m_index]; }

        const value_type* operator->() const { return &m_components[m_index]; }

        const value_type& operator*() const { return m_components[m_index]; }
    };

    iterator begin() { return iterator(m_free, m_objects, 0); }

    iterator end() { return iterator(m_free, m_objects, m_objects.size()); }

   private:
    std::vector<value_type> m_objects;
    std::vector<bool> m_free;

    const size_t m_chunk_size;
};

} /* namespace dm */

#endif /* end of include guard: OBJECTPOOL_HPP */
