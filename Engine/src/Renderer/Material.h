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
        std::string name;
        SharedPtr<Texture2D> texturePtr { nullptr };
    };

public:
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
    const lia::vec3& GetEmissionColor() const { return m_emissionColor; }
    float GetShininess() const { return m_shininess; }

    void ApplyMaterial(const SharedPtr<Shader>& shader) const;

private:
    void LoadTexture(const std::string& dir, const bool async, TextureMap& map, TextureSlot slot);

private:
    TextureMap m_textures[TextureSlot::Count];
    lia::vec3 m_ambientColor;
    lia::vec3 m_diffuseColor;
    lia::vec3 m_specularColor;
    lia::vec3 m_emissionColor;

    float m_shininess { 32.0f };
};

} // namespace elv
