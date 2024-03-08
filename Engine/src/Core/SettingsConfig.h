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
    bool enableSaveScene { false };
    bool enableEditor { false };
    bool enableFullscreen { false };
    bool enableVSync { false };
    bool enableFpsCounter { false };
    bool enableZSorting { false };
    bool enableSceneGraph { false };

    // orthographic camera
    float orthographicCameraSize { 100 };
};

extern SettingsConfig gEngineSettings;

} // namespace elv
