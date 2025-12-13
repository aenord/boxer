#include "engine/gfx/SpriteSheet.h"
#include "engine/utils/JsonParser.h"
#include <SDL3/SDL_log.h>

namespace engine {

bool SpriteSheet::LoadFromFile(const std::string& jsonPath) {
    // Parse JSON file
    JsonValue root;
    if (!JsonParser::ParseFile(jsonPath, root)) {
        SDL_Log("Failed to parse sprite sheet: %s", jsonPath.c_str());
        return false;
    }
    
    if (!root.IsObject()) {
        SDL_Log("Invalid sprite sheet JSON (expected object): %s", jsonPath.c_str());
        return false;
    }
    
    // Get texture path
    if (!root.HasKey("texture") || !root["texture"].IsString()) {
        SDL_Log("Sprite sheet missing 'texture' field: %s", jsonPath.c_str());
        return false;
    }
    
    // Get directory of JSON file for relative texture path
    std::string dir;
    size_t lastSlash = jsonPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        dir = jsonPath.substr(0, lastSlash + 1);
    }
    
    std::string texturePath = dir + root["texture"].AsString();
    m_texture = std::make_shared<Texture2D>(texturePath);
    if (!m_texture->IsValid()) {
        SDL_Log("Failed to load sprite sheet texture: %s", texturePath.c_str());
        return false;
    }
    
    // Parse sprites
    if (!root.HasKey("sprites") || !root["sprites"].IsObject()) {
        SDL_Log("Sprite sheet missing 'sprites' object: %s", jsonPath.c_str());
        return false;
    }
    
    float texWidth = static_cast<float>(m_texture->GetWidth());
    float texHeight = static_cast<float>(m_texture->GetHeight());
    
    const JsonValue& sprites = root["sprites"];
    for (const auto& [name, spriteValue] : sprites) {
        if (!spriteValue.IsObject()) continue;
        
        if (!spriteValue.HasKey("x") || !spriteValue.HasKey("y") ||
            !spriteValue.HasKey("w") || !spriteValue.HasKey("h")) {
            SDL_Log("Sprite '%s' missing x/y/w/h fields", name.c_str());
            continue;
        }
        
        int x = spriteValue["x"].AsInt();
        int y = spriteValue["y"].AsInt();
        int w = spriteValue["w"].AsInt();
        int h = spriteValue["h"].AsInt();
        
        // Convert pixel coords to normalized UVs
        // Note: Y is flipped because stb_image flips for OpenGL
        float minU = x / texWidth;
        float maxU = (x + w) / texWidth;
        float minV = 1.0f - (y + h) / texHeight;
        float maxV = 1.0f - y / texHeight;
        
        m_sprites[name] = Sprite(Vec4(minU, minV, maxU, maxV), w, h);
    }
    
    SDL_Log("Loaded sprite sheet '%s' with %zu sprites", jsonPath.c_str(), m_sprites.size());
    return true;
}

void SpriteSheet::SetTexture(std::shared_ptr<Texture2D> texture) {
    m_texture = std::move(texture);
}

void SpriteSheet::AddSprite(const std::string& name, int x, int y, int width, int height) {
    if (!m_texture || !m_texture->IsValid()) {
        SDL_Log("Cannot add sprite: no valid texture set");
        return;
    }
    
    float texWidth = static_cast<float>(m_texture->GetWidth());
    float texHeight = static_cast<float>(m_texture->GetHeight());
    
    // Convert pixel coords to normalized UVs (Y flipped for OpenGL)
    float minU = x / texWidth;
    float maxU = (x + width) / texWidth;
    float minV = 1.0f - (y + height) / texHeight;
    float maxV = 1.0f - y / texHeight;
    
    m_sprites[name] = Sprite(Vec4(minU, minV, maxU, maxV), width, height);
}

const Sprite* SpriteSheet::GetSprite(const std::string& name) const {
    auto it = m_sprites.find(name);
    return it != m_sprites.end() ? &it->second : nullptr;
}

bool SpriteSheet::HasSprite(const std::string& name) const {
    return m_sprites.find(name) != m_sprites.end();
}

} // namespace engine
