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

    j.at("window_height").get_to(WindowHeight);
    j.at("window_width").get_to(WindowWidth);
    j.at("default_scene_name").get_to(DefaultSceneName);
    j.at("load_default_scene").get_to(LoadDefaultScene);

    EL_CORE_INFO("Settings loaded");
}

void SettingsConfig::SaveSettings()
{
    json j;
    j["window_height"] = WindowHeight;
    j["window_width"] = WindowWidth;
    j["default_scene_name"] = DefaultSceneName;
    j["load_default_scene"] = LoadDefaultScene;

    std::ofstream out(engineSettingsFile.data());
    out << std::setfill(' ') << std::setw(2) << j;
}
} // namespace elv
