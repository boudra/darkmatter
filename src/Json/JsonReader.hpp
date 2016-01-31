#ifndef JSON_READER_HPP
#define JSON_READER_HPP

#include "JsonNode.hpp"

#include <fstream>

namespace dm {

class JsonReader {
   public:
    JsonReader() {}

    JsonNode ParseValue();
    Any ParseNumber();
    std::string ParseString();
    JsonNode ParseObject();
    JsonNode ParseArray();

    void PrintError(const std::string &message);

    inline const bool EndOfFile() const { return m_data >= m_end; }

    inline void SkipWhiteSpaces() {
        while ((*m_data == ' ' || *m_data == '\n' || *m_data == '\r' ||
                *m_data == '\t') &&
               !EndOfFile())
            m_data++;
    }

    inline void SkipUntil(const char &c) {
        while (*m_data != c && !EndOfFile()) m_data++;
    }

    void PrettyPrint(JsonNode *node);

    JsonNode Parse(const char *filename);
    JsonNode Parse(std::ifstream &file);

    virtual ~JsonReader() {}

   private:
    size_t m_size;

    char *m_data;
    char *m_start;
    char *m_end;
};
}

#endif