#include "AudioManager.h"
#include "Core/FileSystem.h"

#include "Core/Timer.h"

#include <irrKlang.h>

namespace elv {

AudioManager gAudioManager;

void AudioManager::Init()
{
    m_engine = irrklang::createIrrKlangDevice();
}

void AudioManager::Shutdown()
{
    for (auto it : m_sounds) {
        if (it.second) {
            it.second->drop();
        }
    }

    m_engine->drop();
}

void AudioManager::AddSound(const std::string& name, const std::string& path)
{
    Timer timer;
    irrklang::ISound* sound = m_engine->play2D(std::format("{}{}", FileSystem::GetSoundsPath(), path).c_str(), false, true, true);
    EL_CORE_INFO("Audio file {0} is loaded in {1} ms.", name, timer.ElapsedMs());
    if (sound) {
        m_sounds.insert({ name, sound });
    }
}

void AudioManager::SetVolume(const std::string& name, float volume)
{
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        it->second->setVolume(volume);
    } else {
        EL_CORE_WARN("Set volume failed: {0} sound isn't  exist.", name);
    }
}

void AudioManager::Play(const std::string& name, bool looped)
{
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        auto sound = it->second;
        if (sound->isFinished()) {
            m_engine->play2D(sound->getSoundSource(), looped, false, true);
        } else if (sound->getIsPaused()) {
            sound->setIsPaused(false);
        }
    } else {
        EL_CORE_WARN("Audio play failed: {0} isn't exist.", name);
    }
}

void AudioManager::Pause(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        it->second->setIsPaused(true);
    } else {
        EL_CORE_WARN("Audio play failed: {0} isn't exist.", name);
    }
}

void AudioManager::Stop(const std::string& name)
{
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        it->second->stop();
    } else {
        EL_CORE_WARN("Audio play failed: {0} isn't exist.", name);
    }
}
} // namespace elv
