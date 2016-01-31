#include "JsonReader.hpp"

#include <iostream>
#include <cstring>

namespace dm {

JsonNode JsonReader::ParseValue() {
    this->SkipWhiteSpaces();
    JsonNode node;

    /* String */
    if (*m_data == '"' || *m_data == '\'') {
        std::string string = this->ParseString();
        node.SetValue(std::move(string));
        node.SetType(JsonNode::Type::Value);
    }

    /* Number */
    else if ((*m_data >= '0' && *m_data <= '9') || *m_data == '-') {
        Any value = this->ParseNumber();
        node.SetValue(std::move(value));
        node.SetType(JsonNode::Type::Value);
    }

    /* Object */
    else if (*m_data == '{') {
        node = this->ParseObject();
        node.SetType(JsonNode::Type::Object);
    }

    /* Array */
    else if (*m_data == '[') {
        node = this->ParseArray();
        node.SetType(JsonNode::Type::Array);
    } else if (std::strncmp(m_data, "true", 4) == 0 ||
               std::strncmp(m_data, "TRUE", 4) == 0) {
        node.SetValue(true);
        node.SetType(JsonNode::Type::Value);
        m_data += 4;
    } else if (std::strncmp(m_data, "false", 5) == 0 ||
               std::strncmp(m_data, "FALSE", 5) == 0) {
        node.SetValue(false);
        node.SetType(JsonNode::Type::Value);
        m_data += 5;
    } else if (std::strncmp(m_data, "null", 4) == 0 ||
               std::strncmp(m_data, "NULL", 4) == 0) {
        node.SetValue(nullptr);
        node.SetType(JsonNode::Type::Value);
        m_data += 4;
    } else {
        PrintError(
            "Syntax Error: Expected a String, Number, Bool, Object, Array or "
            "null.");
    }

    return node;
}

Any JsonReader::ParseNumber() {
    char *start = m_data;
    Any number_value;
    char last_char;

    bool foundcomma = false;

    if (*m_data == '-') m_data++;

    while (!EndOfFile()) {
        if ((*m_data >= '0' && *m_data <= '9')) {
            m_data++;
        } else if (*m_data == '.') {
            if (foundcomma) {
                PrintError("Syntax Error: Expected a number");
                break;
            } else {
                foundcomma = true;
                m_data++;
            }

        } else {
            break;
        }
    }

    last_char = *m_data;
    *m_data = '\0';

    if (foundcomma) {
        number_value = Any(std::strtod(start, nullptr));
    } else {
        number_value = (m_data - start > 4)
                           ? Any(std::strtol(start, nullptr, 10))
                           : Any(std::stoi(start, nullptr, 10));
    }

    *m_data = last_char;

    return number_value;
}

std::string JsonReader::ParseString() {
    char quote = *m_data++;
    char *start = m_data;
    unsigned int stringlength = 0;
    std::string result;

    /* Loop until we find the ending quote */
    while (*m_data != quote || (*m_data == quote && *(m_data - 1) == '\\')) {
        m_data++;
        stringlength++;
    }

    m_data++;

    /* Reserve enough space to store the string */
    result.reserve(stringlength);
    result.assign(start, stringlength);

    return result;
}

JsonNode JsonReader::ParseObject() {
    JsonNode object = JsonNode(JsonNode::Type::Object);
    bool end = false;

    if (*m_data == '{') m_data++;

    /* Look for the next key name */
    while (!end) {
        std::string key;

        /* Look for string quotes */
        while (*m_data != '\"' && *m_data != '\'' && !EndOfFile()) m_data++;

        if (EndOfFile()) {
            PrintError("Syntax Error : '\"' , ''' were expected. ");
            break;
        }

        /* Parse the key as string */
        key = ParseString();

        SkipUntil(':');

        if (EndOfFile()) {
            PrintError("Syntax Error : ':' was expected. ");
            break;
        }

        m_data++;

        object[key] = this->ParseValue();

        while (*m_data != ',' && *m_data != '}' && !EndOfFile()) m_data++;

        switch (*m_data) {
            case ',':

                m_data++;
                break;

            case '}':

                end = true;
                m_data++;
                break;

            default:

                PrintError("Syntax Error : ',' , '}' were expected. ");
                end = true;
                break;
        };
    }

    return object;
}

JsonNode JsonReader::ParseArray() {
    JsonNode object = JsonNode(JsonNode::Type::Array);
    bool end = false;

    /* Verify we're parsing an array */
    if (*m_data == '[') m_data++;

    while (!end) {
        /* Use the index value as the key name */
        object[object.ChildCount()] = this->ParseValue();

        while (*m_data != ',' && *m_data != ']' && !EndOfFile()) m_data++;

        switch (*m_data) {
            case ',':
                m_data++;
                break;

            case ']':
                end = true;
                m_data++;
                break;

            default:
                PrintError("Syntax Error : ',' , ']' were expected. ");
                end = true;
                break;
        };
    }

    return object;
}

void JsonReader::PrintError(const std::string &message) {
    unsigned int lines = 0;
    unsigned int column = 0;

    /* Count lines and columns until we reach our current position */
    for (char *data = m_start; data < m_data; data++) {
        column++;

        if (*data == '\n') {
            lines++;
            column = 0;
        }
    }

    std::cout << "[Line: " << ++lines << ", Col: " << ++column << "] "
              << message << std::endl;
}

JsonNode JsonReader::Parse(std::ifstream &file) {
    std::string keyname;

    file.seekg(0, std::ios::end);
    m_size = static_cast<size_t>(file.tellg());

    m_data = new char[m_size];

    m_start = m_data;
    m_end = m_start + m_size;

    file.seekg(0, std::ios::beg);
    file.read(m_data, m_size);
    file.close();

    this->SkipWhiteSpaces();

    JsonNode root;

    /* The root element of a JSON document has to be an object or an array. */
    if (*m_data == '[') {
        root = this->ParseArray();
    } else if (*m_data == '{') {
        root = this->ParseObject();
    } else {
        PrintError("Syntax Error : '{' , '[' were expected. ");
    }

    /* clean up */
    delete[] m_start;

    return root;
}

JsonNode JsonReader::Parse(const char *filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Fatal Error: File \"" << filename << "\" doesn't exist."
                  << std::endl;
        return JsonNode();
    } else {
        return this->Parse(file);
    }
}
}
