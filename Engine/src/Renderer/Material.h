#pragma once

namespace elv {
class Texture2D;
class Shader;

class Material {
public:
    enum TextureSlot {
        Diffuse,
        Specular,
        Normal,
        Emission,
        Transparency,

        Count
    };

private:
    struct TextureMap {
        bool needReload { false };
        std::string name;
        SharedPtr<Texture2D> texturePtr { nullptr };
    };

public:
    Material();

    /**
     * Set already loaded texture texture to the slot
     *
     * @param slot Texture slot
     * @param name Texture name
     * @param texture already loaded texture pointer
     */
    void SetTexture(const TextureSlot slot, const std::string& name, SharedPtr<Texture2D> texture);

    /**
     * Set texture info to the slot, texture will be loaded on LoadTextures function call
     *
     * @param slot Texture slot
     * @param name Texture name
     */
    void SetTexture(const TextureSlot slot, const std::string& name);

    /**
     * Set texture info to the slot and immediately call loading
     *
     * @param slot Texture slot
     * @param name Texture name
     * @param dir Texture path
     * @param async Async texture loading (true by default)
     */
    void SetTexture(const TextureSlot slot, const std::string& name, const std::string& dir, bool async);

    // Load all textures
    void LoadTextures(const std::string& dir, const bool async);

    void SetAmbientColor(const lia::vec3& color);
    void SetDiffuseColor(const lia::vec3& color);
    void SetSpecularColor(const lia::vec3& color);
    void SetEmissionColor(const lia::vec3& color);
    void SetShininess(const float shininess);

    const lia::vec3& GetAmbientColor() const { return m_ambientColor; }
    const lia::vec3& GetDiffuseColor() const { return m_diffuseColor; }
    const lia::vec3& GetSpecularColor() const { return m_specularColor; }
    const lia::vec3& GetEmissionColor() const { return m_emissiveColor; }
    float GetShininess() const { return m_shininess; }

    const std::string& GetTextureName(TextureSlot slot) const { return m_textures[slot].name; }

    void ApplyMaterial(const SharedPtr<Shader>& shader) const;

private:
    void LoadTexture(const std::string& dir, const bool async, TextureMap& map);

private:
    TextureMap m_textures[TextureSlot::Count];

    // Ambient & diffuse color is enabled by default
    lia::vec3 m_ambientColor { 1.0f };
    lia::vec3 m_diffuseColor { 1.0f };

    // Enabled by default but texture is disabled for properly calculating specular texture color.
    // To enable: set white texture to the specular texture slot
    lia::vec3 m_specularColor { 1.0f };

    // Enabled by default but texture is disabled for properly calculating emission texture color.
    // To enable: set white texture to the emission texture slot
    lia::vec3 m_emissiveColor { 1.0f };

    float m_shininess { 32.0f };
};

} // namespace elv
