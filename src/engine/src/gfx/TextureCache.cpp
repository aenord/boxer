#include "engine/gfx/TextureCache.h"
#include <SDL3/SDL_log.h>

namespace engine {

std::shared_ptr<Texture2D> TextureCache::Load(const std::string& path, TextureFilter filter) {
    // Check if already cached and still alive
    auto it = m_cache.find(path);
    if (it != m_cache.end()) {
        if (auto existing = it->second.lock()) {
            return existing;
        }
        // weak_ptr expired, will reload below (i.e. texture is no longer in use)
    }

    // Cache miss: load from disk
    auto texture = std::make_shared<Texture2D>(path, filter);
    if (!texture->IsValid()) {
        SDL_Log("TextureCache: Failed to load '%s'", path.c_str());
        return nullptr;
    }

    m_cache[path] = texture;
    SDL_Log("TextureCache: Loaded '%s' (%dx%d)", 
            path.c_str(), texture->GetWidth(), texture->GetHeight());
    return texture;
}

void TextureCache::Preload(const std::vector<std::string>& paths, TextureFilter filter) {
    for (const auto& path : paths) {
        Load(path, filter);
    }
}

void TextureCache::Clear() {
    m_cache.clear();
}

size_t TextureCache::GetCachedCount() const {
    size_t count = 0;
    for (const auto& [path, weak] : m_cache) {
        if (!weak.expired()) {
            ++count;
        }
    }
    return count;
}

} // namespace engine

