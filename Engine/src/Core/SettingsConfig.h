#pragma once

namespace Elven {

class SettingsConfig {
public:
    void LoadSettings();
    void SaveSettings();

public:
    uint16_t WindowHeight { 720 };
    uint16_t WindowWidth { 1280 };
    std::string DefaultSceneName { "default" };
};

extern SettingsConfig gEngineSettings;

} // namespace Elven
