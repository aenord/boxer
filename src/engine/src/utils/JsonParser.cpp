#include "engine/utils/JsonParser.h"
#include <SDL3/SDL_log.h>
#include <fstream>
#include <sstream>
#include <cctype>

namespace engine {

// Static null value for invalid accesses
const JsonValue JsonValue::s_null;
const std::string JsonValue::s_emptyString;

// JsonValue accessors

bool JsonValue::AsBool(bool defaultVal) const {
    if (m_type == Type::Bool) return m_bool;
    if (m_type == Type::Number) return m_number != 0.0;
    return defaultVal;
}

int JsonValue::AsInt(int defaultVal) const {
    return m_type == Type::Number ? static_cast<int>(m_number) : defaultVal;
}

float JsonValue::AsFloat(float defaultVal) const {
    return m_type == Type::Number ? static_cast<float>(m_number) : defaultVal;
}

double JsonValue::AsDouble(double defaultVal) const {
    return m_type == Type::Number ? m_number : defaultVal;
}

const std::string& JsonValue::AsString() const {
    return m_type == Type::String ? m_string : s_emptyString;
}

size_t JsonValue::Size() const {
    if (m_type == Type::Array) return m_array.size();
    if (m_type == Type::Object) return m_object.size();
    return 0;
}

const JsonValue& JsonValue::operator[](size_t index) const {
    if (m_type == Type::Array && index < m_array.size()) {
        return m_array[index];
    }
    return s_null;
}

bool JsonValue::HasKey(const std::string& key) const {
    return m_type == Type::Object && m_object.find(key) != m_object.end();
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    if (m_type == Type::Object) {
        auto it = m_object.find(key);
        if (it != m_object.end()) {
            return it->second;
        }
    }
    return s_null;
}

// JsonParser implementation

bool JsonParser::ParseFile(const std::string& path, JsonValue& outValue) {
    std::ifstream file(path);
    if (!file.is_open()) {
        SDL_Log("JsonParser: Failed to open file '%s'", path.c_str());
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return ParseString(buffer.str(), outValue);
}

bool JsonParser::ParseString(const std::string& json, JsonValue& outValue) {
    size_t pos = 0;
    if (!ParseValue(json, pos, outValue)) {
        return false;
    }
    
    // Check for trailing garbage (allow whitespace)
    SkipWhitespace(json, pos);
    if (pos < json.size()) {
        SDL_Log("JsonParser: Unexpected data after JSON value at position %zu", pos);
        return false;
    }
    
    return true;
}

void JsonParser::SkipWhitespace(const std::string& json, size_t& pos) {
    while (pos < json.size()) {
        char c = json[pos];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            ++pos;
        } else {
            break;
        }
    }
}

bool JsonParser::ParseValue(const std::string& json, size_t& pos, JsonValue& outValue) {
    SkipWhitespace(json, pos);
    
    if (pos >= json.size()) {
        SDL_Log("JsonParser: Unexpected end of input");
        return false;
    }
    
    char c = json[pos];
    
    if (c == '"') {
        outValue.m_type = JsonValue::Type::String;
        return ParseString(json, pos, outValue.m_string);
    }
    if (c == '{') {
        return ParseObject(json, pos, outValue);
    }
    if (c == '[') {
        return ParseArray(json, pos, outValue);
    }
    if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
        outValue.m_type = JsonValue::Type::Number;
        return ParseNumber(json, pos, outValue.m_number);
    }
    if (ParseLiteral(json, pos, "true")) {
        outValue.m_type = JsonValue::Type::Bool;
        outValue.m_bool = true;
        return true;
    }
    if (ParseLiteral(json, pos, "false")) {
        outValue.m_type = JsonValue::Type::Bool;
        outValue.m_bool = false;
        return true;
    }
    if (ParseLiteral(json, pos, "null")) {
        outValue.m_type = JsonValue::Type::Null;
        return true;
    }
    
    SDL_Log("JsonParser: Unexpected character '%c' at position %zu", c, pos);
    return false;
}

bool JsonParser::ParseString(const std::string& json, size_t& pos, std::string& outStr) {
    if (pos >= json.size() || json[pos] != '"') {
        return false;
    }
    ++pos; // skip opening quote
    
    outStr.clear();
    while (pos < json.size()) {
        char c = json[pos];
        
        if (c == '"') {
            ++pos; // skip closing quote
            return true;
        }
        
        if (c == '\\') {
            ++pos;
            if (pos >= json.size()) {
                SDL_Log("JsonParser: Unterminated escape sequence");
                return false;
            }
            char escaped = json[pos];
            switch (escaped) {
                case '"':  outStr += '"'; break;
                case '\\': outStr += '\\'; break;
                case '/':  outStr += '/'; break;
                case 'b':  outStr += '\b'; break;
                case 'f':  outStr += '\f'; break;
                case 'n':  outStr += '\n'; break;
                case 'r':  outStr += '\r'; break;
                case 't':  outStr += '\t'; break;
                case 'u':
                    // Unicode escape - skip for simplicity, just consume 4 hex digits
                    pos += 4;
                    if (pos >= json.size()) return false;
                    outStr += '?'; // Placeholder
                    break;
                default:
                    outStr += escaped;
                    break;
            }
        } else {
            outStr += c;
        }
        ++pos;
    }
    
    SDL_Log("JsonParser: Unterminated string");
    return false;
}

bool JsonParser::ParseNumber(const std::string& json, size_t& pos, double& outNum) {
    size_t start = pos;
    
    // Optional minus
    if (pos < json.size() && json[pos] == '-') ++pos;
    
    // Integer part
    if (pos >= json.size()) return false;
    if (json[pos] == '0') {
        ++pos;
    } else if (std::isdigit(static_cast<unsigned char>(json[pos]))) {
        while (pos < json.size() && std::isdigit(static_cast<unsigned char>(json[pos]))) ++pos;
    } else {
        return false;
    }
    
    // Fractional part
    if (pos < json.size() && json[pos] == '.') {
        ++pos;
        if (pos >= json.size() || !std::isdigit(static_cast<unsigned char>(json[pos]))) {
            return false;
        }
        while (pos < json.size() && std::isdigit(static_cast<unsigned char>(json[pos]))) ++pos;
    }
    
    // Exponent part
    if (pos < json.size() && (json[pos] == 'e' || json[pos] == 'E')) {
        ++pos;
        if (pos < json.size() && (json[pos] == '+' || json[pos] == '-')) ++pos;
        if (pos >= json.size() || !std::isdigit(static_cast<unsigned char>(json[pos]))) {
            return false;
        }
        while (pos < json.size() && std::isdigit(static_cast<unsigned char>(json[pos]))) ++pos;
    }
    
    try {
        outNum = std::stod(json.substr(start, pos - start));
        return true;
    } catch (...) {
        SDL_Log("JsonParser: Invalid number at position %zu", start);
        return false;
    }
}

bool JsonParser::ParseObject(const std::string& json, size_t& pos, JsonValue& outValue) {
    if (pos >= json.size() || json[pos] != '{') return false;
    ++pos; // skip {
    
    outValue.m_type = JsonValue::Type::Object;
    outValue.m_object.clear();
    
    SkipWhitespace(json, pos);
    
    // Empty object
    if (pos < json.size() && json[pos] == '}') {
        ++pos;
        return true;
    }
    
    while (pos < json.size()) {
        SkipWhitespace(json, pos);
        
        // Parse key
        std::string key;
        if (!ParseString(json, pos, key)) {
            SDL_Log("JsonParser: Expected string key in object");
            return false;
        }
        
        SkipWhitespace(json, pos);
        
        // Expect colon
        if (pos >= json.size() || json[pos] != ':') {
            SDL_Log("JsonParser: Expected ':' after object key");
            return false;
        }
        ++pos;
        
        // Parse value
        JsonValue value;
        if (!ParseValue(json, pos, value)) {
            return false;
        }
        
        outValue.m_object[key] = std::move(value);
        
        SkipWhitespace(json, pos);
        
        if (pos >= json.size()) {
            SDL_Log("JsonParser: Unterminated object");
            return false;
        }
        
        if (json[pos] == '}') {
            ++pos;
            return true;
        }
        
        if (json[pos] != ',') {
            SDL_Log("JsonParser: Expected ',' or '}' in object");
            return false;
        }
        ++pos; // skip comma
    }
    
    SDL_Log("JsonParser: Unterminated object");
    return false;
}

bool JsonParser::ParseArray(const std::string& json, size_t& pos, JsonValue& outValue) {
    if (pos >= json.size() || json[pos] != '[') return false;
    ++pos; // skip [
    
    outValue.m_type = JsonValue::Type::Array;
    outValue.m_array.clear();
    
    SkipWhitespace(json, pos);
    
    // Empty array
    if (pos < json.size() && json[pos] == ']') {
        ++pos;
        return true;
    }
    
    while (pos < json.size()) {
        JsonValue element;
        if (!ParseValue(json, pos, element)) {
            return false;
        }
        
        outValue.m_array.push_back(std::move(element));
        
        SkipWhitespace(json, pos);
        
        if (pos >= json.size()) {
            SDL_Log("JsonParser: Unterminated array");
            return false;
        }
        
        if (json[pos] == ']') {
            ++pos;
            return true;
        }
        
        if (json[pos] != ',') {
            SDL_Log("JsonParser: Expected ',' or ']' in array");
            return false;
        }
        ++pos; // skip comma
    }
    
    SDL_Log("JsonParser: Unterminated array");
    return false;
}

bool JsonParser::ParseLiteral(const std::string& json, size_t& pos, const char* literal) {
    size_t len = std::strlen(literal);
    if (pos + len > json.size()) return false;
    
    if (json.compare(pos, len, literal) == 0) {
        pos += len;
        return true;
    }
    return false;
}

} // namespace engine

