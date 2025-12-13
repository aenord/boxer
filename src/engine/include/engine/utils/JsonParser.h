#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace engine {

/**
 * Minimal JSON parser for configuration files.
 * Supports: objects, arrays, strings, numbers, booleans, null.
 * 
 * Usage:
 *   JsonValue root;
 *   if (JsonParser::ParseFile("config.json", root)) {
 *       std::string name = root["name"].AsString();
 *       int count = root["count"].AsInt();
 *   }
 */

class JsonValue {
public:
    enum class Type { Null, Bool, Number, String, Array, Object };
    
    JsonValue() : m_type(Type::Null) {}
    
    // Type checking
    Type GetType() const { return m_type; }
    bool IsNull() const { return m_type == Type::Null; }
    bool IsBool() const { return m_type == Type::Bool; }
    bool IsNumber() const { return m_type == Type::Number; }
    bool IsString() const { return m_type == Type::String; }
    bool IsArray() const { return m_type == Type::Array; }
    bool IsObject() const { return m_type == Type::Object; }
    
    // Value accessors (with defaults for type mismatches)
    bool AsBool(bool defaultVal = false) const;
    int AsInt(int defaultVal = 0) const;
    float AsFloat(float defaultVal = 0.0f) const;
    double AsDouble(double defaultVal = 0.0) const;
    const std::string& AsString() const;
    
    // Array access
    size_t Size() const;
    const JsonValue& operator[](size_t index) const;
    
    // Object access
    bool HasKey(const std::string& key) const;
    const JsonValue& operator[](const std::string& key) const;
    
    // Iterators for objects
    auto begin() const { return m_object.begin(); }
    auto end() const { return m_object.end(); }

private:
    friend class JsonParser;
    
    Type m_type = Type::Null;
    bool m_bool = false;
    double m_number = 0.0;
    std::string m_string;
    std::vector<JsonValue> m_array;
    std::unordered_map<std::string, JsonValue> m_object;
    
    static const JsonValue s_null;
    static const std::string s_emptyString;
};

class JsonParser {
public:
    /**
     * Parse JSON from a file.
     * Returns true on success, false on error.
     */
    static bool ParseFile(const std::string& path, JsonValue& outValue);
    
    /**
     * Parse JSON from a string.
     * Returns true on success, false on error.
     */
    static bool ParseString(const std::string& json, JsonValue& outValue);

private:
    static bool Parse(const std::string& json, size_t& pos, JsonValue& outValue);
    static void SkipWhitespace(const std::string& json, size_t& pos);
    static bool ParseValue(const std::string& json, size_t& pos, JsonValue& outValue);
    static bool ParseString(const std::string& json, size_t& pos, std::string& outStr);
    static bool ParseNumber(const std::string& json, size_t& pos, double& outNum);
    static bool ParseObject(const std::string& json, size_t& pos, JsonValue& outValue);
    static bool ParseArray(const std::string& json, size_t& pos, JsonValue& outValue);
    static bool ParseLiteral(const std::string& json, size_t& pos, const char* literal);
};

} // namespace engine

