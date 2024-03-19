#include "Material.h"

#include "Shader.h"
#include "Texture2D.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Resources/TextureManager.h"

namespace elv {

constexpr const char* kDefaultTextureMapNames[Material::TextureSlot::Count] = {
    "texture_diffuse",
    "texture_specular",
    "texture_normal",
    "texture_emission",
    "texture_transparency"
};

Material::Material()
{
    // set default textures
    m_textures[TextureSlot::Diffuse].name = "white";
    m_textures[TextureSlot::Specular].name = "black";
    m_textures[TextureSlot::Emission].name = "black";
    m_textures[TextureSlot::Transparency].name = "white";

    LoadTexture("", false, m_textures[TextureSlot::Diffuse]);
    LoadTexture("", false, m_textures[TextureSlot::Specular]);
    LoadTexture("", false, m_textures[TextureSlot::Emission]);
    LoadTexture("", false, m_textures[TextureSlot::Transparency]);
}

void Material::SetTexture(const TextureSlot slot, const std::string& name, SharedPtr<Texture2D> texture)
{
    auto& map = m_textures[slot];
    map.name = name;
    map.texturePtr = texture;
}

void Material::SetTexture(const TextureSlot slot, const std::string& name)
{
    if (name.empty()) {
        EL_CORE_ERROR("Failed to set material texture, texture name is empty.");
        return;
    }

    auto& map = m_textures[slot];
    map.name = name;
    map.needReload = true;
}

void Material::SetTexture(const TextureSlot slot, const std::string& name, const std::string& dir, bool async)
{
    if (name.empty()) {
        EL_CORE_ERROR("Failed to set material texture, texture name is empty.");
        return;
    }

    auto& map = m_textures[slot];
    map.name = name;

    LoadTexture(dir, async, map);
}

void Material::LoadTextures(const std::string& dir, const bool async)
{
    for (int i = 0; i < TextureSlot::Count; ++i) {
        auto& textureMap = m_textures[i];
        if (textureMap.needReload || (!textureMap.texturePtr && !textureMap.name.empty())) {
            LoadTexture(dir, async, textureMap);
            textureMap.needReload = false;
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
    m_emissiveColor = color;
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
            shader->SetInteger(fmt::format("u_Material.{}", kDefaultTextureMapNames[i]), i);
        }
    }

    shader->SetVector3f("u_Material.ambientColor", m_ambientColor);
    shader->SetVector3f("u_Material.diffuseColor", m_diffuseColor);
    shader->SetVector3f("u_Material.specularColor", m_specularColor);
    shader->SetVector3f("u_Material.emissionColor", m_emissiveColor);
    shader->SetFloat("u_Material.shininess", m_shininess);
}

void Material::LoadTexture(const std::string& dir, const bool async, Material::TextureMap& map)
{
    auto readyTexturePtr = textures::Get(map.name);
    if (readyTexturePtr) {
        map.texturePtr = readyTexturePtr;
    } else {
        textures::Load(map.name, fmt::format("{}/{}", dir, map.name), async);

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
