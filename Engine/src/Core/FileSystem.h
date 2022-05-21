#pragma once

namespace Elven
{
    class FileSystem
    {
    public:
        static const char* GetAssetsPath() { return "assets/"; }
        static const char* GetShadersPath() { return "assets/shaders/"; }
        static const char* GetImagesPath() { return "assets/images/"; }
    };
}