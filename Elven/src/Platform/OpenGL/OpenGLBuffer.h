#pragma once
#include "Elven/Renderer/Buffer.h"
namespace Elven
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        void SetData(const void* data, uint32_t size) override;

        const VertexBufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }

    private:
        VertexBufferLayout m_Layout;
        uint32_t m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();

        void Bind() const;
        void Unbind() const;

        uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}

