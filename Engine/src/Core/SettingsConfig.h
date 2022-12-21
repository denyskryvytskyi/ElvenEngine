#pragma once

namespace Elven {

class SettingsConfig {
public:
    void LoadSettings();

public:
    uint16_t WindowHeight;
    uint16_t WindowWidth;
    std::string DefaultSceneName;
};

extern SettingsConfig gEngineSettings;

} // namespace Elven
