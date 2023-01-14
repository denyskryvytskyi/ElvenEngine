#pragma once

namespace elv {

class SettingsConfig {
public:
    void LoadSettings();
    void SaveSettings();

public:
    uint16_t WindowHeight { 720 };
    uint16_t WindowWidth { 1280 };
    std::string DefaultSceneName { "default" };
    bool LoadDefaultScene { false };
};

extern SettingsConfig gEngineSettings;

} // namespace elv
