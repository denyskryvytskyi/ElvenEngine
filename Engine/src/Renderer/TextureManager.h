#pragma once

#include "Texture2D.h"

#include <future>
#include <set>

namespace elv {

class TextureManager {
public:
    struct LoadedTextureInfo {
        int width { 0 };
        int height { 0 };
        int nrChannels { 0 };
        unsigned char* data { nullptr };
        std::string textureName;
    };

public:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    const TextureManager& operator=(const TextureManager&) = delete;

    // async load from file
    void Load(const std::string& textureName, const std::string& filename);

    // just create texture for specific texture implementation
    SharedPtr<Texture2D> Load(const std::string& textureName, std::uint32_t width, std::uint32_t height);

    void OnUpdate();
    void Shutdown();

    SharedPtr<Texture2D> Get(const std::string& textureName);

private:
    void CreateTexture(const LoadedTextureInfo& info);

private:
    std::unordered_map<std::string, SharedPtr<Texture2D>> m_textures;
    std::vector<LoadedTextureInfo> m_loadedInfo;
    std::set<std::string> m_textureLoadingInProgress;
    std::vector<std::future<void>> m_futures;
};

extern TextureManager gTextureManager;

namespace textures {

inline void Load(const std::string& textureName, const std::string& filename)
{
    gTextureManager.Load(textureName, filename);
}

inline SharedPtr<Texture2D> Load(const std::string& textureName, std::uint32_t width, std::uint32_t height)
{
    return gTextureManager.Load(textureName, width, height);
}

inline SharedPtr<Texture2D> Get(const std::string& textureName)
{
    return gTextureManager.Get(textureName);
}

} // namespace textures
} // namespace elv
