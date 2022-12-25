#include <json.hpp>
#include <type_traits>

#include "SettingsConfig.h"
namespace Elven {
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

    WindowHeight = j.at("window_height");
    WindowWidth = j.at("window_width");
    DefaultSceneName = j.at("default_scene_name");

    EL_CORE_INFO("Settings loaded");
}

void SettingsConfig::SaveSettings()
{
    json j;
    j["window_height"] = WindowHeight;
    j["window_width"] = WindowWidth;
    j["default_scene_name"] = DefaultSceneName;

    std::ofstream out(engineSettingsFile.data());
    out << j;
}
} // namespace Elven
