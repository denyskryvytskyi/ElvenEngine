#pragma once

namespace Elven
{
    enum class BufferAttributeType
    {
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

    static uint32_t BufferAttributeTypeSize(BufferAttributeType type)
    {
        switch (type)
        {
        case BufferAttributeType::Float: return sizeof(float);
        case BufferAttributeType::Float2: return 2 * sizeof(float);
        case BufferAttributeType::Float3: return 3 * sizeof(float);
        case BufferAttributeType::Float4: return 4 * sizeof(float);
        case BufferAttributeType::Int: return sizeof(int);
        case BufferAttributeType::Int2: return 2 * sizeof(int);
        case BufferAttributeType::Int3: return 3 * sizeof(int);
        case BufferAttributeType::Int4: return 4 * sizeof(int);
        case BufferAttributeType::Bool: return sizeof(bool);
        }

        EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
        return 0;
    }

    struct VertexBufferAttribute
    {
        std::string Name;
        size_t Offset;
        uint32_t Size;
        BufferAttributeType Type;
        bool Normalized;

        VertexBufferAttribute() = default;

        VertexBufferAttribute(BufferAttributeType type, const std::string& name, bool normalized = false);

        uint32_t GetComponentsCount() const
        {
            switch (Type)
            {
            case BufferAttributeType::Float: return 1;
            case BufferAttributeType::Float2: return 2;
            case BufferAttributeType::Float3: return 3;
            case BufferAttributeType::Float4: return 4;
            case BufferAttributeType::Int: return 1;
            case BufferAttributeType::Int2: return 2;
            case BufferAttributeType::Int3: return 3;
            case BufferAttributeType::Int4: return 4;
            case BufferAttributeType::Bool: return 1;
            }

            EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
            return 0;
        }
    };


    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;

        VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes);

        uint32_t GetStride() const { return m_Stride; }
        const std::vector<VertexBufferAttribute>& GetAttributes() const { return m_Attributes; }

        std::vector<VertexBufferAttribute>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexBufferAttribute>::iterator end() { return m_Attributes.end(); }
        std::vector<VertexBufferAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
        std::vector<VertexBufferAttribute>::const_iterator end() const { return m_Attributes.end(); }

    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& attribute : m_Attributes)
            {
                attribute.Offset = offset;
                offset += attribute.Size;
                m_Stride += attribute.Size;
            }
        }

    private:
        std::vector<VertexBufferAttribute> m_Attributes;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual void SetLayout(const VertexBufferLayout& layout) = 0;
        virtual const VertexBufferLayout& GetLayout() const = 0;

        static VertexBuffer* Create(uint32_t size);
        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
}

