#include "AudioManager.h"
#include "Core/FileSystem.h"

#include "Core/Profiler.h"
#include "Core/Timer.h"

#if MODULE_SOUND_ENABLED
#    include <irrKlang.h>
#endif

namespace elv {

AudioManager gAudioManager;

void AudioManager::Init()
{
#if MODULE_SOUND_ENABLED
    m_engine = irrklang::createIrrKlangDevice();
#else
    EL_CORE_WARN("Sound module is disabled!")
#endif
}

void AudioManager::Shutdown()
{
#if MODULE_SOUND_ENABLED
    for (auto& it : m_sounds) {
        if (it.second.sound) {
            it.second.sound->stop();
            it.second.sound->drop();
            it.second.sound = nullptr;
            it.second.soundSource = nullptr;
        }
    }

    m_engine->drop();
#else
#endif
}

void AudioManager::AddSound(const std::string& name, const std::string& path)
{
#if MODULE_SOUND_ENABLED
    irrklang::ISoundSource* source = nullptr;
    {
        PROFILE_TO_LOG(fmt::format("Audio file {} is loaded in:", name));
        source = m_engine->addSoundSourceFromFile(fmt::format("{}{}", fileSystem::SOUNDS_PATH, path).c_str(), irrklang::ESM_AUTO_DETECT, true);
    }
    if (source) {
        m_sounds.insert({ name, { nullptr, source } });
    }
#endif
}

void AudioManager::SetVolume(const std::string& name, float volume)
{
#if MODULE_SOUND_ENABLED
    auto it = m_sounds.find(name);
    if (it != m_sounds.end() && it->second.sound) {
        it->second.sound->setVolume(volume);
    } else {
        EL_CORE_WARN("Set volume failed: {0} sound isn't  exist.", name);
    }
#endif
}

void AudioManager::Play(const std::string& name, bool looped)
{
#if MODULE_SOUND_ENABLED
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        if (it->second.sound) {
            auto sound = it->second.sound;
            if (sound->isFinished()) {
                m_engine->play2D(sound->getSoundSource(), looped, false, false);
            } else if (sound->getIsPaused()) {
                sound->setIsPaused(false);
            }
        } else {
            // first play of the sound using preloaded sound source
            it->second.sound = m_engine->play2D(it->second.soundSource, looped, false, true, true);
        }
    } else {
        EL_CORE_WARN("Audio play failed: {0} isn't exist.", name);
    }
#endif
}

void AudioManager::Pause(const std::string& name)
{
#if MODULE_SOUND_ENABLED
    auto it = m_sounds.find(name);
    if (it != m_sounds.end() && it->second.sound) {
        it->second.sound->setIsPaused(true);
    } else {
        EL_CORE_WARN("Audio pause failed: {0} isn't exist.", name);
    }
#endif
}

void AudioManager::Stop(const std::string& name)
{
#if MODULE_SOUND_ENABLED
    auto it = m_sounds.find(name);
    if (it != m_sounds.end() && it->second.sound) {
        it->second.sound->stop();
        it->second.sound->drop();
        it->second.sound = nullptr;
    } else {
        EL_CORE_WARN("Audio stop failed: {0} isn't exist.", name);
    }
#endif
}
std::vector<std::string> AudioManager::GetSounds() const
{
    std::vector<std::string> names;
#if MODULE_SOUND_ENABLED

    for (const auto soundInfo : m_sounds) {
        names.emplace_back(soundInfo.first);
    }

#endif
    return names;
}
} // namespace elv
