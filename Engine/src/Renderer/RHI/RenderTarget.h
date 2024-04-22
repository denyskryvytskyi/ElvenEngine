#pragma once

namespace elv {

class Texture;

class RenderTarget {
public:
    struct Size {
        std::uint32_t Width { 0 };
        std::uint32_t Height { 0 };
    };

public:
    RenderTarget() = default;
    virtual ~RenderTarget() = default;

    virtual void Init(const std::uint32_t width, const std::uint32_t height, bool isMultisampled = false) = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void BindDraw() const = 0;
    virtual void BindRead() const = 0;
    virtual void Resize(const std::uint32_t width, const std::uint32_t height) = 0;
    virtual void Blit(const UniquePtr<RenderTarget>& src) = 0;

    void BindColorTexture(const int slot) const;
    std::shared_ptr<Texture> GetColorTextureAttachment() const;
    const Size& GetSize() const { return m_size; }

    static UniquePtr<RenderTarget> Create();

protected:
    bool m_isMultisampled { false };
    Size m_size;
    std::shared_ptr<Texture> m_colorTextureAttachment { nullptr };
    std::shared_ptr<Texture> m_depthStencilAttachment { nullptr };
};

} // namespace elv
