#pragma once

#include "Renderer/VertexArray.h"

namespace Elven
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        void AddVertexBuffer(VertexBuffer* vertexBuffer) override;
        void SetIndexBuffer(IndexBuffer* indexBuffer) override;

        virtual void Bind() const override;

        virtual uint32_t GetId() const override { return m_id; }
        virtual uint32_t GetIndexCount() const override { return m_indexBuffer ? m_indexBuffer->GetCount() : 0; }

    private:
        std::vector<VertexBuffer*> m_vertexBuffers;
        IndexBuffer* m_indexBuffer = nullptr;
        uint32_t m_vertexBufferIndex = 0;
        uint32_t m_id = 0;
    };
}

