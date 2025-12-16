#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "engine/gfx/Texture2D.h"

namespace engine {

// Caches loaded textures to prevent redundant uploads to the GPU
// Returns shared_ptr so that multiple users can share the same texture.
class TextureCache {
public:
    TextureCache() = default;
    ~TextureCache() = default;

    // Non-copyable
    TextureCache(const TextureCache&) = delete;
    TextureCache& operator=(const TextureCache&) = delete;

    // Loads texture & returns cached version if available
    std::shared_ptr<Texture2D> Load(const std::string& path, 
                                     TextureFilter filter = TextureFilter::Linear);

    // Preloads multiple textures (for loading screens)
    void Preload(const std::vector<std::string>& paths, 
                 TextureFilter filter = TextureFilter::Linear);

    // Clears all cached textures
    void Clear();

    // Returns number of textures currently in cache (excluding expired entries)
    size_t GetCachedCount() const;

private:
    std::unordered_map<std::string, std::weak_ptr<Texture2D>> m_cache;
};

} // namespace engine

