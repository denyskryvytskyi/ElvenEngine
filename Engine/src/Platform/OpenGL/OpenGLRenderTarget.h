#pragma once

#include "Renderer/RHI/RenderTarget.h"

namespace elv {

class OpenGLRenderTarget : public RenderTarget {
public:
    struct Size {
        std::uint32_t Width { 0 };
        std::uint32_t Height { 0 };
    };

public:
    OpenGLRenderTarget() = default;
    ~OpenGLRenderTarget();

    virtual void Init(const std::uint32_t width, const std::uint32_t height, bool isMultisampled = false) override;
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void BindDraw() const override;
    virtual void BindRead() const override;
    virtual void Resize(const std::uint32_t width, const std::uint32_t height) override;
    virtual void Blit(const UniquePtr<RenderTarget>& src) override;

private:
    void InitAttachments();

private:
    std::uint32_t m_id { 0 };
};

} // namespace elv
