#pragma once

namespace Elven {

enum class BufferAttributeType {
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static std::uint32_t BufferAttributeTypeSize(BufferAttributeType type)
{
    switch (type) {
    case BufferAttributeType::Float:
        return sizeof(float);
    case BufferAttributeType::Float2:
        return 2 * sizeof(float);
    case BufferAttributeType::Float3:
        return 3 * sizeof(float);
    case BufferAttributeType::Float4:
        return 4 * sizeof(float);
    case BufferAttributeType::Int:
        return sizeof(int);
    case BufferAttributeType::Int2:
        return 2 * sizeof(int);
    case BufferAttributeType::Int3:
        return 3 * sizeof(int);
    case BufferAttributeType::Int4:
        return 4 * sizeof(int);
    case BufferAttributeType::Bool:
        return sizeof(bool);
    }

    EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
    return 0;
}

struct VertexBufferAttribute {
public:
    VertexBufferAttribute() = default;
    VertexBufferAttribute(BufferAttributeType type, bool normalized = false)
        : m_size(BufferAttributeTypeSize(type))
        , m_type(type)
        , m_normalized(normalized)
    {
    }

    std::uint32_t GetComponentsCount() const
    {
        switch (m_type) {
        case BufferAttributeType::Float:
            return 1;
        case BufferAttributeType::Float2:
            return 2;
        case BufferAttributeType::Float3:
            return 3;
        case BufferAttributeType::Float4:
            return 4;
        case BufferAttributeType::Int:
            return 1;
        case BufferAttributeType::Int2:
            return 2;
        case BufferAttributeType::Int3:
            return 3;
        case BufferAttributeType::Int4:
            return 4;
        case BufferAttributeType::Bool:
            return 1;
        }

        EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
        return 0;
    }

public:
    size_t m_offset { 0 };
    std::uint32_t m_size { 0 };
    BufferAttributeType m_type;
    bool m_normalized { false };
};

class VertexBufferLayout {
public:
    VertexBufferLayout() = default;
    VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes)
        : m_attributes(attributes)
    {
        size_t offset = 0;
        for (auto& attribute : m_attributes) {
            attribute.m_offset = offset;
            offset += attribute.m_size;
            m_stride += attribute.m_size;
        }
    }

    std::uint32_t GetStride() const { return m_stride; }
    const std::vector<VertexBufferAttribute>& GetAttributes() const { return m_attributes; }

    std::vector<VertexBufferAttribute>::iterator begin() { return m_attributes.begin(); }
    std::vector<VertexBufferAttribute>::iterator end() { return m_attributes.end(); }
    std::vector<VertexBufferAttribute>::const_iterator begin() const { return m_attributes.begin(); }
    std::vector<VertexBufferAttribute>::const_iterator end() const { return m_attributes.end(); }

private:
    std::vector<VertexBufferAttribute> m_attributes;
    std::uint32_t m_stride { 0 };
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void SetData(const void* data, std::uint32_t size) = 0;
    virtual void SetLayout(const VertexBufferLayout& layout) = 0;

    virtual const std::uint32_t GetId() const = 0;
    virtual const VertexBufferLayout& GetLayout() const = 0;

    static SharedPtr<VertexBuffer> Create(std::uint32_t size);
    static SharedPtr<VertexBuffer> Create(float* vertices, std::uint32_t size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    virtual const std::uint32_t GetId() const = 0;
    virtual std::uint32_t GetCount() const = 0;

    static SharedPtr<IndexBuffer> Create(std::uint32_t* indices, std::uint32_t count);
};

} // namespace Elven
