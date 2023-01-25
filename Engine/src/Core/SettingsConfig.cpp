#include <json.hpp>

#include "SettingsConfig.h"

namespace elv {
using json = nlohmann::json;

namespace {
constexpr std::string_view engineSettingsFile = "settings.cfg";
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

    try {

        j.at("window_height").get_to(windowHeight);
        j.at("window_width").get_to(windowWidth);
        j.at("default_scene_name").get_to(defaultSceneName);
        j.at("load_default_scene").get_to(loadDefaultScene);
        j.at("orthographic_camera_size").get_to(orthographicCameraSize);
    } catch (nlohmann::json_abi_v3_11_2::detail::out_of_range e) {
        EL_ERROR("Load settings config error: {}", e.what());
    }

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

    std::ofstream out(engineSettingsFile.data());
    out << std::setfill(' ') << std::setw(2) << j;
}
} // namespace elv
