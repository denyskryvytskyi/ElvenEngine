#include "TextureManager.h"
#include "Renderer.h"

#include "Core/FileSystem.h"
#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

#include "stb_image.h"

namespace Elven {

TextureManager gTextureManager;

namespace {
std::mutex texturesMutex;

static void LoadTextureFromFile(std::vector<TextureManager::LoadedTextureInfo>& texturesInfo, const std::string& textureName, const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true);

    TextureManager::LoadedTextureInfo info;
    info.textureName = textureName;
    info.data = stbi_load(filepath.c_str(), &info.width, &info.height, &info.nrChannels, 0);

    if (info.data == nullptr) {
        EL_CORE_ASSERT(false, "Failed to load texture!");
    }

    std::lock_guard<std::mutex> lock(texturesMutex);
    texturesInfo.emplace_back(std::move(info));
}
} // namespace

void TextureManager::Load(const std::string& textureName, const std::string& filename)
{
    // check whether we already loaded this texture
    auto it = m_textures.find(textureName);

    if (it == m_textures.end()) {
        // check whether this texture is in loading state
        auto inProgressTexture = m_textureLoadingInProgress.find(textureName);

        if (inProgressTexture == m_textureLoadingInProgress.end()) {
            if (it == m_textures.end()) {
                m_textureLoadingInProgress.insert(textureName);
                const std::string filepath = FileSystem::GetImagesPath() + filename;
                m_futures.push_back(std::async(std::launch::async, LoadTextureFromFile, std::ref(m_loadedInfo), textureName, filepath));
            } else {
                EL_CORE_INFO("Texture {0} is already loaded.", textureName);
            }
        }
    }
}

SharedPtr<Texture2D> TextureManager::Load(const std::string& textureName, std::uint32_t width, std::uint32_t height)
{
    // check whether we already loaded this texture
    auto it = m_textures.find(textureName);

    if (it == m_textures.end()) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL: {
            SharedPtr<Texture2D> texture = MakeSharedPtr<OpenGLTexture2D>(width, height);
            m_textures.insert({ textureName, texture });
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

void TextureManager::OnUpdate()
{
    // create texture with already loaded data
    std::lock_guard<std::mutex> lock(texturesMutex);
    for (auto textureInfo : m_loadedInfo) {
        CreateTexture(textureInfo);
        m_textureLoadingInProgress.erase(textureInfo.textureName);
    }

    for (auto& info : m_loadedInfo) {
        stbi_image_free(info.data);
    }
    m_loadedInfo.clear();
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

SharedPtr<Texture2D> TextureManager::Get(const std::string& textureName)
{
    auto it = m_textures.find(textureName);
    return it != m_textures.end() ? it->second : nullptr;
}

void TextureManager::CreateTexture(const LoadedTextureInfo& info)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL: {

        SharedPtr<Texture2D> texture = MakeSharedPtr<OpenGLTexture2D>(info.width, info.height, info.nrChannels);
        texture->SetData(info.data);

        m_textures.insert({ info.textureName, std::move(texture) });

        UniquePtr<events::Event> e = MakeUniquePtr<events::TextureLoadedEvent>(info.textureName);
        events::QueueEvent(std::move(e), string_id(info.textureName));
        break;
    }
    default:
        EL_CORE_ASSERT(false, "Unknown Renderer API!");
    }
}

} // namespace Elven
