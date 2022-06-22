#pragma once

namespace Elven {

class Texture2D {
public:
    virtual void BindToUnit(std::uint32_t unit) = 0;
    virtual void SetData(void* data) = 0;

    static SharedPtr<Texture2D> Create(const std::string& filepath);
    static SharedPtr<Texture2D> Create(std::uint32_t width, std::uint32_t height);
};

} // namespace Elven
