#pragma once

#include "Renderer/RHI/Texture.h"

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

        void Free() const;
    };

public:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    const TextureManager& operator=(const TextureManager&) = delete;

    // async load from file
    void Load(const std::string& textureName, const std::string& filePath, const bool isAsync);

    // load and return info without caching
    LoadedTextureInfo LoadWithoutCache(const std::string& textureName, const std::string& filePath, const bool isFlipped);

    // just create texture for specific texture implementation
    SharedPtr<Texture> Load(const std::string& textureName, std::uint32_t width, std::uint32_t height, uint32_t nrChannels, bool save = true);
    SharedPtr<Texture> Load(const std::uint32_t width, const std::uint32_t height, const Texture::Info& info);

    void Init();
    void Update();
    void Shutdown();

    SharedPtr<Texture> Get(std::string_view textureName);

    std::vector<std::string> GetTextureNames() const;

private:
    void CreateTexture(const LoadedTextureInfo& info);

private:
    std::unordered_map<std::string, SharedPtr<Texture>> m_textures;
    std::vector<LoadedTextureInfo> m_loadedInfo;
    std::set<std::string> m_textureLoadingInProgress;
    std::vector<std::future<void>> m_futures;
};

extern TextureManager gTextureManager;

namespace textures {

inline void Load(const std::string& textureName, const std::string& filePath, const bool async = true)
{
    gTextureManager.Load(textureName, filePath, async);
}

inline SharedPtr<Texture> Load(const std::string& textureName, std::uint32_t width, std::uint32_t height, uint32_t nrChannels = 3)
{
    return gTextureManager.Load(textureName, width, height, nrChannels);
}

inline SharedPtr<Texture> Load(std::uint32_t width, std::uint32_t height, const Texture::Info& info)
{
    return gTextureManager.Load(width, height, info);
}

inline SharedPtr<Texture> LoadUnique(const std::string& textureName, std::uint32_t width, std::uint32_t height, uint32_t nrChannels = 3)
{
    return gTextureManager.Load(textureName, width, height, nrChannels, false);
}

inline TextureManager::LoadedTextureInfo LoadWithoutCache(const std::string& textureName, const std::string& filePath, const bool isFlipped = true)
{
    return gTextureManager.LoadWithoutCache(textureName, filePath, isFlipped);
}

inline SharedPtr<Texture> Get(std::string_view textureName)
{
    return gTextureManager.Get(textureName);
}

} // namespace textures
} // namespace elv
