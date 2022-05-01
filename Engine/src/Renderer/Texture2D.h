#pragma once

namespace Elven
{
    class Texture2D
    {
        struct Properties
        {

        };
    public:
        virtual void Bind() = 0;
        virtual void SetData(void* data) = 0;

        static SharedPtr<Texture2D> Create(const char* filepath);
        static SharedPtr<Texture2D> Create(uint32_t width, uint32_t height);
    };
}