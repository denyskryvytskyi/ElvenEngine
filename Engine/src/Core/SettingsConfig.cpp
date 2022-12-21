#include <json.hpp>
#include <type_traits>

#include "SettingsConfig.h"
namespace Elven {
using json = nlohmann::json;

namespace {
constexpr std::string_view engineSettingsFile = "engine_settings.cfg";
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
            EL_CORE_ERROR("Failed to load engine settings {0}", engineSettingsFile);
            return;
        }
    }

    WindowHeight = j.at("window_height");
    WindowWidth = j.at("window_width");
    DefaultSceneName = j.at("default_scene_name");

    EL_CORE_INFO("Settings loaded");
}
} // namespace Elven
