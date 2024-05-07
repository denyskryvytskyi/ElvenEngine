#pragma once

namespace elv {

class Texture {
public:
    enum class WrappingMode {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class InternalFormat {
        RGBA8,
        RGB8,
        R8,
        DepthStencil
    };

    enum class DataType {
        UnsignedByte,
        UnsignedInt32
    };

    struct Info {
        InternalFormat InternalFormat { InternalFormat::RGBA8 };
        DataType DataType { DataType::UnsignedByte };
        WrappingMode WrapS { WrappingMode::ClampToEdge };
        WrappingMode WrapT { WrappingMode::ClampToEdge };
        WrappingMode WrapR { WrappingMode::ClampToEdge };

        bool Multisampled { false };
    };

public:
    virtual ~Texture() = default;

    virtual void BindToSlot(std::uint32_t slot) = 0;
    virtual void Unbind() = 0;
    virtual void SetData(void* data, bool generateMipmap = true) = 0;
    virtual void SetWrappingMode(const WrappingMode wrappingMode) = 0;

    virtual std::uint32_t GetId() const = 0;

    static SharedPtr<Texture> Create(std::uint32_t width, std::uint32_t height, const Info& info);
    static SharedPtr<Texture> Create(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels = 3);
};

} // namespace elv
