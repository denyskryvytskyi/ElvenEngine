#pragma once

#include <filesystem>

namespace elv {

class FileSystem {
public:
    static const char* GetAssetsPath() { return "assets/"; }
    static const char* GetShadersPath() { return "assets/shaders/"; }
    static const char* GetImagesPath() { return "assets/images/"; }
    static const char* GetScenesPath() { return "assets/scenes/"; }
    static const char* GetSoundsPath() { return "assets/sounds/"; }
    static std::string GetCurrentPath() { return std::filesystem::current_path().string(); }
};

} // namespace elv
