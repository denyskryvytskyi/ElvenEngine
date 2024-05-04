#pragma once

#if MODULE_SOUND_ENABLED
namespace irrklang {
class ISoundEngine;
class ISoundSource;
class ISound;
}; // namespace irrklang
#endif

namespace elv {

class AudioManager {
private:
    struct SoundInfo {
#if MODULE_SOUND_ENABLED
        irrklang::ISound* sound { nullptr };
        irrklang::ISoundSource* soundSource { nullptr };
#endif
    };

public:
    AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    const AudioManager& operator=(const AudioManager&) = delete;

public:
    void Init();
    void Shutdown();

    void AddSound(const std::string& name, const std::string& path);
    void SetVolume(const std::string& name, float volume);
    void Play(const std::string& name, bool looped = false);
    void Pause(const std::string& name);
    void Stop(const std::string& name);

    std::vector<std::string> GetSounds() const;

private:
#if MODULE_SOUND_ENABLED
    irrklang::ISoundEngine* m_engine { nullptr };
#endif
    std::unordered_map<std::string, SoundInfo> m_sounds;
};

extern AudioManager gAudioManager;
} // namespace elv
