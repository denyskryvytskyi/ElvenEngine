#pragma once

#include "Renderer/RHI/Buffer.h"
#include "Renderer/RHI/VertexArray.h"

namespace elv {

class OpenGL41VertexArray : public VertexArray {
public:
    OpenGL41VertexArray();
    ~OpenGL41VertexArray() override;

    void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) override;

    void Bind() const override;

    void Unbind() const;

    std::uint32_t GetId() const override { return m_id; }
    std::uint32_t GetIndexCount() const override { return m_indexBuffer ? m_indexBuffer->GetCount() : 0; }

private:
    std::vector<SharedPtr<VertexBuffer>> m_vertexBuffers;
    SharedPtr<IndexBuffer> m_indexBuffer;
    std::uint32_t m_vertexBufferIndex { 0 };
    std::uint32_t m_id { 0 };
};

} // namespace elv
