#pragma once

#include "Elven/Renderer/Buffer.h"

namespace Elven
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
        virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

        virtual const std::vector<VertexBuffer*>& GetVertexBuffer() const = 0;
        virtual const IndexBuffer* GetIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}

