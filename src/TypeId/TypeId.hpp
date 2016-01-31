#ifndef TYPEID_HPP
#define TYPEID_HPP

#include <cstdint>
#include <string>
#include <type_traits>

#define dm_type_get(type) dm::global_type_info<dm_type_filter(type)>

#define dm_type_filter(type_)                               \
    typename std::remove_reference<typename std::remove_cv< \
        typename std::remove_pointer<type_>::type>::type>::type

#define dm_register_type_name(type, name_)      \
    namespace dm {                              \
    dm_internal_register_type_name(type, name_) \
    }

#define dm_internal_register_type_name(type, name_)                      \
    template <>                                                          \
    struct global_type_info<dm_type_filter(type)> {                      \
        static constexpr const char *name = name_;                       \
        static uint32_t id() {                                           \
            static uint32_t current_id = type_info_counter::counter()++; \
            return current_id;                                           \
        }                                                                \
    };

#define dm_register_type(type) dm_register_type_name(type, #type)
#define dm_type_id(type) dm_type_get(type)::id()
#define dm_type_name(type) dm_type_get(type)::name
#define dm_type_info(type) TypeInfo(dm_type_id(type), dm_type_name(type))

namespace dm {

struct type_info_counter {
    static uint32_t &counter() {
        static uint32_t c = 1;
        return c;
    }
};

template <typename type>
struct global_type_info {
    static constexpr const char *name = "unknown";
    static constexpr uint32_t id() { return 0; }
};

class TypeInfo {
   public:
    TypeInfo(const uint32_t id, const std::string name)
        : m_id(id), m_name(name) {}

    template <typename type>
    TypeInfo()
        : TypeInfo(global_type_info<type>::id, global_type_info<type>::name) {}

    ~TypeInfo() {}

    const std::string &name() const { return m_name; }

    uint32_t id() const { return m_id; }

    bool operator==(const TypeInfo &typeinfo) const {
        return m_id == typeinfo.m_id;
    }

    bool operator!=(const TypeInfo &typeinfo) const {
        return m_id != typeinfo.m_id;
    }

    bool operator==(const std::string &str) const { return m_name == str; }

    bool operator!=(const std::string &str) const { return m_name != str; }

    bool is(const std::string &str) const { return m_name == str; }

    template <typename type>
    inline bool is() const {
        return m_id == dm_type_id(type);
    }

   private:
    uint32_t m_id;
    std::string m_name;
};
}

dm_register_type(int) dm_register_type(char) dm_register_type(float)
    dm_register_type(double) dm_register_type(long) dm_register_type(bool)
        dm_register_type_name(std::nullptr_t, "null")

#endif
