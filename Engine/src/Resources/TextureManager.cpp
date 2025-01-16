#include "TextureManager.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

#include "Core/Profiler.h"

#include <stb/stb_image.h>

namespace elv {

TextureManager gTextureManager;

namespace {
std::mutex texturesMutex;

static void LoadTextureFromFile(std::vector<TextureManager::LoadedTextureInfo>& texturesInfo, const std::string& textureName, const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true);

    TextureManager::LoadedTextureInfo info;
    info.textureName = textureName;

    {
        PROFILE_TO_LOG(fmt::format("Texture {} is loaded in", textureName.c_str()))
        info.data = stbi_load(filepath.data(), &info.width, &info.height, &info.nrChannels, 0);
    }

    if (info.data == nullptr) {
        EL_CORE_CRITICAL("Failed to load texture: {0}", textureName);
        return;
    }

    std::lock_guard<std::mutex> lock(texturesMutex);
    texturesInfo.emplace_back(std::move(info));
}
} // namespace

void TextureManager::Load(const std::string& textureName, const std::string& filePath, const bool isAsync)
{
    // check whether we already loaded this texture
    auto it = m_textures.find(textureName);

    if (it == m_textures.end()) {
        // check whether this texture is in loading state
        auto inProgressTexture = m_textureLoadingInProgress.find(textureName);

        if (inProgressTexture == m_textureLoadingInProgress.end()) {
            m_textureLoadingInProgress.insert(textureName);
            if (isAsync) {
                m_futures.push_back(std::async(std::launch::async, LoadTextureFromFile, std::ref(m_loadedInfo), textureName, filePath));
            } else {
                LoadTextureFromFile(m_loadedInfo, textureName, filePath);
            }
        }
        return;
    }

    EL_CORE_INFO("Texture {0} is already loaded.", textureName);
}

TextureManager::LoadedTextureInfo TextureManager::LoadWithoutCache(const std::string& textureName, const std::string& filePath, const bool isFlipped)
{
    LoadedTextureInfo info;
    stbi_set_flip_vertically_on_load(isFlipped);

    {
        PROFILE_TO_LOG(fmt::format("Texture {} is loaded in", textureName.c_str()))
        info.data = stbi_load(filePath.data(), &info.width, &info.height, &info.nrChannels, 0);
    }

    if (info.data == nullptr) {
        EL_CORE_CRITICAL("Failed to load texture: {0}", textureName);
    }
    return info;
}

SharedPtr<Texture> TextureManager::Load(const std::string& textureName, std::uint32_t width, std::uint32_t height, uint32_t nrChannels, bool save)
{
    // check whether we already loaded this texture
    auto it = m_textures.find(textureName);

    if (it == m_textures.end()) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL: {
            SharedPtr<Texture> texture = MakeSharedPtr<OpenGLTexture>(width, height, nrChannels);
            if (save) {
                m_textures.insert({ textureName, texture });
            }
            return texture;
        }
        default:
            EL_CORE_ASSERT(false, "Unknown Renderer API!");
        }
    } else {
        EL_CORE_INFO("Texture {0} is already loaded.", textureName);
        return it->second;
    }

    return nullptr;
}

SharedPtr<Texture> TextureManager::Load(const std::uint32_t width, const std::uint32_t height, const Texture::Info& info)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        SharedPtr<Texture> texture = MakeSharedPtr<OpenGLTexture>(width, height, info);
        return texture;
    }

    EL_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

void TextureManager::Init()
{
    SharedPtr<Texture> whiteTexture = Load("white", 1, 1, 3);
    unsigned char whiteData[] = { 255, 255, 255 }; // RGB format
    whiteTexture->SetData(&whiteData, false);

    SharedPtr<Texture> blackTexture = Load("black", 1, 1, 3);
    unsigned char blackData[] = { 0, 0, 0 }; // RGB format
    blackTexture->SetData(&blackData, false);
}

void TextureManager::Update()
{
    // create texture with already loaded data
    if (texturesMutex.try_lock()) {
        for (auto& textureInfo : m_loadedInfo) {
            CreateTexture(textureInfo);
            m_textureLoadingInProgress.erase(textureInfo.textureName);
        }

        for (auto& info : m_loadedInfo) {
            stbi_image_free(info.data);
        }
        m_loadedInfo.clear();
        texturesMutex.unlock();
    }
}

void TextureManager::Shutdown()
{
    for (const auto& futureTask : m_futures) {
        futureTask.wait();
    }

    m_futures.clear();
    m_textures.clear();

    for (auto& info : m_loadedInfo) {
        stbi_image_free(info.data);
    }
    m_loadedInfo.clear();
}

SharedPtr<Texture> TextureManager::Get(std::string_view textureName)
{
    auto it = m_textures.find(textureName.data());
    return it != m_textures.end() ? it->second : nullptr;
}

std::vector<std::string> TextureManager::GetTextureNames() const
{
    std::vector<std::string> names;

    for (const auto& texture : m_textures) {
        names.emplace_back(texture.first);
    }

    return names;
}

void TextureManager::CreateTexture(const LoadedTextureInfo& info)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL: {
        SharedPtr<Texture> texture = MakeSharedPtr<OpenGLTexture>(info.width, info.height, info.nrChannels);
        texture->SetData(info.data);

        m_textures.insert({ info.textureName, std::move(texture) });

        events::TriggerEvent(events::TextureLoadedEvent { info.textureName }, string_id(info.textureName));
        break;
    }
    default:
        EL_CORE_ASSERT(false, "Unknown Renderer API!");
    }
}

void TextureManager::LoadedTextureInfo::Free() const
{
    stbi_image_free(data);
}

} // namespace elv
