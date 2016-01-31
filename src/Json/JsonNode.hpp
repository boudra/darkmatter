#ifndef JSON_NODE_HPP
#define JSON_NODE_HPP

#include "../TypeId/Any.hpp"

#include <map>
#include <iostream>

namespace dm {

class JsonNode {
   public:
    typedef std::map<std::string, JsonNode> JsonNodes;

    enum Type { Object, Array, Value };

    // Constructors

    JsonNode() : m_type(Type::Value) {}

    JsonNode(const Type type) : m_type(type) {}

    JsonNode(const JsonNode &other)
        : m_type(other.m_type),
          m_elements(other.m_elements),
          m_value(other.m_value) {}

    JsonNode(JsonNode &&other) { this->Swap(other); }

    ~JsonNode() {}

    JsonNode &operator=(const JsonNode &a) {
        JsonNode(a).Swap(*this);
        return *this;
    }

    JsonNode &operator=(JsonNode &&a) {
        a.Swap(*this);
        JsonNode().Swap(a);
        return *this;
    }

    JsonNode &Swap(JsonNode &other) {
        std::swap(m_elements, other.m_elements);
        std::swap(m_type, other.m_type);
        m_value.Swap(other.m_value);
        return *this;
    }

    void SetValue(const Any &value) { m_value = value; }

    void SetValue(Any &&value) { m_value = std::forward<Any>(value); }

    const Any &GetValue() const { return m_value; }

    template <typename ValueType>
    const ValueType Cast() {
        return m_value.Cast<ValueType>();
    }

    JsonNode &operator[](const std::string &name);
    JsonNode &operator[](const size_t index);

    const bool Empty() const { return m_value.Empty(); }

    const bool IsArray() const { return m_type == Type::Array; }

    const bool IsObject() const { return m_type == Type::Object; }

    const bool IsValue() const { return m_type == Type::Value; }

    const bool IsInteger() const { return m_value.Type() == dm_type_info(int); }

    const bool IsString() const {
        return m_value.Type() == dm_type_info(std::string);
    }

    const bool IsDouble() const {
        return m_value.Type() == dm_type_info(double);
    }

    JsonNodes &Children() { return m_elements; }

    const size_t ChildCount() const { return m_elements.size(); }

    const Type GetType() const { return m_type; }

    void SetType(const Type &type) { m_type = type; }

   private:
    Type m_type;
    JsonNodes m_elements;
    Any m_value;
};
}

#endif
