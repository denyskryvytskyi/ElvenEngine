#pragma once

#include "Elven/Renderer/VertexArray.h"

namespace Elven
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer> & vertexBuffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer> & indexBuffer) override;

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
        const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
        uint32_t m_RendererId;
        uint32_t m_VertexBufferIndex;
    };
}

