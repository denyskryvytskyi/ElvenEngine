#pragma once

namespace elv {

class SettingsConfig {
public:
    void LoadSettings();
    void SaveSettings();

public:
    uint16_t windowHeight { 720 };
    uint16_t windowWidth { 1280 };
    std::string defaultSceneName { "default" };
    bool loadDefaultScene { false };

    // orthographic camera
    float orthographicCameraSize { 100 };
};

extern SettingsConfig gEngineSettings;

} // namespace elv
