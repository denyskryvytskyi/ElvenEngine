#pragma once

#include "Renderer/RHI/Texture.h"

namespace elv {

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels = 3);
    OpenGLTexture(std::uint32_t width, std::uint32_t height, const Info& info);
    ~OpenGLTexture();

    void BindToSlot(std::uint32_t slot) override;
    void Unbind() override;
    void SetData(void* data, bool generateMipmap = true) override;
    void SetWrappingMode(const WrappingMode wrappingMode) override;

    std::uint32_t GetId() const override { return m_id; }

private:
    uint32_t m_id { 0 };
    uint32_t m_slot { 0 };
    uint32_t m_width { 0 };
    uint32_t m_height { 0 };
    unsigned int m_internalFormat { 0 };
    unsigned int m_dataFormat { 0 };
};

} // namespace elv
