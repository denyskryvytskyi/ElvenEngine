#pragma once

#include "Renderer/Buffer.h"

namespace Elven {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    ~OpenGLVertexBuffer();

    void SetData(const void* data, uint32_t size) override;
    void SetLayout(const VertexBufferLayout& layout_) override { m_layout = layout_; }

    virtual const uint32_t GetId() const override { return m_id; }
    const VertexBufferLayout& GetLayout() const override { return m_layout; }

private:
    VertexBufferLayout m_layout;
    uint32_t m_id = 0;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer();

    virtual const uint32_t GetId() const override { return m_id; }
    uint32_t GetCount() const { return m_count; }

private:
    uint32_t m_id { 0 };
    uint32_t m_count { 0 };
};

} // namespace Elven
