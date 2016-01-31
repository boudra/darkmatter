#include "JsonNode.hpp"

namespace dm {

JsonNode &JsonNode::operator[](const std::string &name) {
    auto it = m_elements.lower_bound(name);

    if (it != m_elements.end() && it->first == name) return it->second;

    if (m_type != Type::Object) m_type = Type::Object;

    it = m_elements.insert(it, std::make_pair(name, JsonNode()));

    return it->second;
}

JsonNode &JsonNode::operator[](const size_t index) {
    std::string index_str = std::to_string(index);
    auto it = m_elements.lower_bound(index_str);

    if (it != m_elements.end() && it->first == index_str) return it->second;

    if (m_type != Type::Array) m_type = Type::Array;

    it =
        m_elements.insert(it, std::make_pair(std::move(index_str), JsonNode()));

    return it->second;
}
}
