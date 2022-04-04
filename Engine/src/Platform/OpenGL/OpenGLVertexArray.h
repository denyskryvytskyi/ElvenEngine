#pragma once

#include "Renderer/VertexArray.h"

namespace Elven
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
        void SetIndexBuffer(IndexBuffer* indexBuffer) override;

        const std::vector<VertexBuffer*>& GetVertexBuffer() const override { return m_VertexBuffers; }
        const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        std::vector<VertexBuffer*> m_VertexBuffers;
        IndexBuffer* m_IndexBuffer = nullptr;
        uint32_t m_RendererId = 0;
        uint32_t m_VertexBufferIndex = 0;
    };
}

