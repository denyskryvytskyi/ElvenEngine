#pragma once

#include "Renderer/Texture2D.h"

namespace Elven
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const char* filepath);
        OpenGLTexture2D(uint32_t width, uint32_t height);

        virtual void Bind() override;
        virtual void SetData(void* data) override;

    private:
        unsigned int m_id { 0 };
        uint32_t m_width { 0 };
        uint32_t m_height { 0 };
        unsigned int m_internalFormat;
        unsigned int m_dataFormat;
    };
}