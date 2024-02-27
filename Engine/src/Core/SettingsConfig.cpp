#include <json.hpp>

#include "SettingsConfig.h"

namespace elv {
using json = nlohmann::json;

namespace {
constexpr std::string_view engineSettingsFile = "settings.cfg";
}

template<typename T>
void LoadSetting(const std::string& name, T& setting, nlohmann::json& j)
{
    try {
        j.at(name).get_to(setting);
    } catch (nlohmann::json_abi_v3_11_2::detail::out_of_range e) {
        EL_CORE_WARN("Failed to load setting: {}", e.what());
    }
}

SettingsConfig gEngineSettings;

void SettingsConfig::LoadSettings()
{
    EL_CORE_INFO("Settings loading...");
    json j;
    {
        std::ifstream in(engineSettingsFile.data());
        if (in.is_open()) {
            in >> j;
        } else {
            EL_CORE_WARN("Failed to load engine settings config {0}. Will be created new one with default values.", engineSettingsFile);
            SaveSettings();
            return;
        }
    }

    LoadSetting("window_height", windowHeight, j);
    LoadSetting("window_height", windowHeight, j);
    LoadSetting("window_width", windowWidth, j);
    LoadSetting("default_scene_name", defaultSceneName, j);
    LoadSetting("load_default_scene", loadDefaultScene, j);
    LoadSetting("orthographic_camera_size", orthographicCameraSize, j);
    LoadSetting("enable_save_scene", enableSaveScene, j);
    LoadSetting("enable_editor", enableEditor, j);
    LoadSetting("enable_fullscreen", enableFullscreen, j);
    LoadSetting("enable_vsync", enableVSync, j);
    LoadSetting("enable_fps_counter", enableFpsCounter, j);
    LoadSetting("enable_z_sorting", enableZSorting, j);

    EL_CORE_INFO("Settings loaded");
}

void SettingsConfig::SaveSettings()
{
    json j;
    j["window_height"] = windowHeight;
    j["window_width"] = windowWidth;
    j["default_scene_name"] = defaultSceneName;
    j["load_default_scene"] = loadDefaultScene;
    j["orthographic_camera_size"] = orthographicCameraSize;
    j["enable_save_scene"] = enableSaveScene;
    j["enable_editor"] = enableEditor;
    j["enable_fullscreen"] = enableFullscreen;
    j["enable_vsync"] = enableVSync;
    j["enable_fps_counter"] = enableFpsCounter;
    j["enable_z_sorting"] = enableZSorting;

    std::ofstream out(engineSettingsFile.data());
    out << std::setfill(' ') << std::setw(2) << j;
}
} // namespace elv
