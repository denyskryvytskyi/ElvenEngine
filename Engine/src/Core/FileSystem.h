#pragma once

#include <filesystem>

namespace elv::fileSystem {

inline constexpr const char* ASSETS_PATH = "assets/";
inline constexpr const char* SHADERS_PATH = "assets/shaders/";
inline constexpr const char* IMAGES_PATH = "assets/images/";
inline constexpr const char* SCENES_PATH = "assets/scenes/";
inline constexpr const char* SOUNDS_PATH = "assets/sounds/";

inline std::string GetCurrentPath() { return std::filesystem::current_path().string(); }
inline bool IsDirectoryExists(const std::string dir) { return std::filesystem::exists(dir); }
inline void CreateDir(const std::string dir) { std::filesystem::create_directory(dir); }

} // namespace elv::fileSystem
