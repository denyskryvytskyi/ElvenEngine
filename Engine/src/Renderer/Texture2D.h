#pragma once

namespace elv {

enum class TextureWrappingMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

class Texture2D {

public:
    virtual ~Texture2D() = default;

    virtual void BindToUnit(std::uint32_t unit) = 0;
    virtual void SetData(void* data, bool generateMipmap = true) = 0;
    virtual void SetWrappingMode(const TextureWrappingMode wrappingMode) = 0;
};

} // namespace elv
