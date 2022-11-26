#pragma once

namespace Elven {

class Texture2D {
public:
    virtual ~Texture2D() = default;

    virtual void BindToUnit(std::uint32_t unit) = 0;
    virtual void SetData(void* data, bool generateMipmap = true) = 0;
};

} // namespace Elven
