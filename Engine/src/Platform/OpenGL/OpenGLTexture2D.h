#pragma once

#include "Renderer/Texture2D.h"

namespace elv {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels = 3);
    ~OpenGLTexture2D();

    void BindToUnit(std::uint32_t unit) override;
    void SetData(void* data, bool generateMipmap = true) override;

private:
    unsigned int m_id { 0 };
    std::uint32_t m_width { 0 };
    std::uint32_t m_height { 0 };
    unsigned int m_internalFormat { 0 };
    unsigned int m_dataFormat { 0 };
};

} // namespace elv
