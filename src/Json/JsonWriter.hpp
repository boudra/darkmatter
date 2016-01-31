#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

#include "JsonNode.hpp"

#include <sstream>

namespace dm {

class JsonWriter {
   public:
    JsonWriter() : m_printlevel(0), m_identWidth(2) {}

    ~JsonWriter() { m_buffer.clear(); }

    std::string PrettyPrint(JsonNode &root) {
        PrintNode(root);
        return m_buffer.str();
    }

    void PrettyPrint(JsonNode &root, std::ostream &out) {
        this->PrintNode(root);
        out << m_buffer.str() << '\n';
        ;
    }

   private:
    void PrintNode(JsonNode &node);

    unsigned int m_printlevel;
    unsigned int m_identWidth;

    std::stringstream m_buffer;
};
}

#endif
