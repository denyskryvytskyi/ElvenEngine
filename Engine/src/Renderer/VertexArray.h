#pragma once

#include "Renderer/Buffer.h"

namespace Elven {

class VertexArray {
public:
    virtual ~VertexArray() = default;

    virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
    virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

    virtual void Bind() const = 0;

    virtual uint32_t GetId() const = 0;
    virtual uint32_t GetIndexCount() const = 0;

    static VertexArray* Create();
};

} // namespace Elven
