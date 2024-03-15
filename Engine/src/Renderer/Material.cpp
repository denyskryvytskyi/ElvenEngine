#include "Material.h"

#include "Shader.h"
#include "Texture2D.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Resources/TextureManager.h"

namespace elv {

static int counter = 0;
static int loadedCounter = 0;

void Material::SetTexture(const TextureSlot slot, const std::string& name, const SharedPtr<Texture2D>& texture)
{
    auto& map = m_textures[slot];
    map.name = name;
    map.texturePtr = texture;
}

void Material::SetTexture(const TextureSlot slot, const std::string& name)
{
    auto& map = m_textures[slot];
    map.name = name;
}

void Material::LoadTextures(const std::string& dir, const bool async)
{
    for (int i = 0; i < TextureSlot::Count; ++i) {
        auto& textureMap = m_textures[i];
        if (!textureMap.texturePtr && !textureMap.name.empty()) {
            LoadTexture(dir, async, textureMap, static_cast<TextureSlot>(i));
        }
    }
}

void Material::SetAmbientColor(const lia::vec3& color)
{
    m_ambientColor = color;
}

void Material::SetDiffuseColor(const lia::vec3& color)
{
    m_diffuseColor = color;
}

void Material::SetSpecularColor(const lia::vec3& color)
{
    m_specularColor = color;
}

void Material::SetEmissionColor(const lia::vec3& color)
{
    m_emissionColor = color;
}

void Material::SetShininess(const float shininess)
{
    m_shininess = shininess;
}

void Material::ApplyMaterial(const SharedPtr<Shader>& shader) const
{
    for (int i = 0; i < TextureSlot::Count; ++i) {
        const auto& textureMap = m_textures[i];
        if (textureMap.texturePtr) {
            textureMap.texturePtr->BindToUnit(i);

            shader->SetInteger(fmt::format("u_Material.{}", textureMap.name), i);
        }
    }

    shader->SetVector3f("u_Material.ambientColor", m_ambientColor);
    shader->SetVector3f("u_Material.diffuseColor", m_diffuseColor);
    shader->SetVector3f("u_Material.specularColor", m_specularColor);
    shader->SetVector3f("u_Material.emissionColor", m_emissionColor);
}

void Material::ResetMaterial() const
{
    // Unbind textures
    for (auto& textureMap : m_textures) {
        if (!textureMap.texturePtr)
            continue;

        textureMap.texturePtr->Unbind();
    }
}

void Material::LoadTexture(const std::string& dir, const bool async, Material::TextureMap& map, Material::TextureSlot slot)
{
    auto readyTexturePtr = textures::Get(map.name);
    if (readyTexturePtr) {
        map.texturePtr = readyTexturePtr;
    } else {
        textures::Load(map.name, fmt::format("{}/{}", dir, map.name), false);

        events::Subscribe<events::TextureLoadedEvent>([&map](const events::TextureLoadedEvent& e) {
            if (!map.name.empty()) {
                if (e.textureName == map.name) {
                    map.texturePtr = textures::Get(map.name);
                }
            }
        },
                                                      string_id(map.name), true);
    }
}

} // namespace elv
