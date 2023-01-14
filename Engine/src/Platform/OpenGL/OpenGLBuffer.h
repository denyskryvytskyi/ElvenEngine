#pragma once

#include "Renderer/Buffer.h"

namespace elv {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(std::uint32_t size);
    OpenGLVertexBuffer(float* data, std::uint32_t size);
    ~OpenGLVertexBuffer() override;

    void SetData(const void* data, std::uint32_t size) override;
    void SetLayout(const VertexBufferLayout& layout_) override { m_layout = layout_; }

    const std::uint32_t GetId() const override { return m_id; }
    const VertexBufferLayout& GetLayout() const override { return m_layout; }

private:
    VertexBufferLayout m_layout;
    std::uint32_t m_id { 0 };
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(std::uint32_t* indices, std::uint32_t count);
    ~OpenGLIndexBuffer() override;

    const std::uint32_t GetId() const override { return m_id; }
    std::uint32_t GetCount() const override { return m_count; }

private:
    std::uint32_t m_id { 0 };
    std::uint32_t m_count { 0 };
};

} // namespace elv
