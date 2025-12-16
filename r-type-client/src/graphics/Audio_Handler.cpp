/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Audio Handler System Implementation
*/

#include "Audio_Handler.hpp"
#include <iostream>
#include <algorithm>

AudioHandlerSystem::~AudioHandlerSystem() {
    Cleanup();
}

void AudioHandlerSystem::Update_currentMusic(MusicSource& music) {
    currentMusic = std::make_unique<MusicSource>(music);
    ::SetMusicVolume(currentMusic->music, currentMusic->volume);
    if (currentMusic->isLoaded) {
        UpdateMusicStream(currentMusic->music);
    }
}

void AudioHandlerSystem::Update_currentSound(SoundSource& sound) {
    currentSound = std::make_unique<SoundSource>(sound);
    ::SetSoundVolume(currentSound->sound, currentSound->volume);
}

void AudioHandlerSystem::play_sound() {
    if (LoadSound()) {
        PlaySound();
    }
}

void AudioHandlerSystem::play_music() {
    if (LoadMusic()) {
        PlayMusic();
    }
}

void AudioHandlerSystem::Initialize() {
    if (!initialized) {
        InitAudioDevice();
        audioInitialized = true;
        initialized = true;
        std::cout << "AudioHandlerSystem: Initialized" << std::endl;
    }
}

void AudioHandlerSystem::Update() {
    if (currentMusic && currentMusic->playing) {
        ::UpdateMusicStream(currentMusic->music);
        if (::IsMusicStreamPlaying(currentMusic->music) && currentMusic->loop && ::GetMusicTimePlayed(currentMusic->music) >= ::GetMusicTimeLength(currentMusic->music)) {
            ::SeekMusicStream(currentMusic->music, 0.0f);
        }
    }
}

void AudioHandlerSystem::Cleanup() {
    if (initialized) {
        UnloadSound();
        UnloadMusic();
        if (audioInitialized) {
            CloseAudioDevice();
            audioInitialized = false;
        }
        initialized = false;
        std::cout << "AudioHandlerSystem: Cleaned up" << std::endl;
    }
}

bool AudioHandlerSystem::LoadSound() {
    currentSound->sound = ::LoadSound(currentSound->pathfile.c_str());
    currentSound->isLoaded = ::IsSoundValid(currentSound->sound);

    if (currentSound->isLoaded) {
        std::cout << "AudioHandlerSystem: Loaded sound '" << currentSound->name << "' from " << currentSound->pathfile << std::endl;
        return true;
    } else {
        std::cout << "AudioHandlerSystem: Failed to load sound '" << currentSound->name << "' from " << currentSound->pathfile << std::endl;
        return false;
    }
}

bool AudioHandlerSystem::LoadMusic() {
    if (!currentMusic) return false;
    currentMusic->music = ::LoadMusicStream(currentMusic->pathfile.c_str());
    currentMusic->isLoaded = ::IsMusicValid(currentMusic->music);

    if (currentMusic->isLoaded) {
        ::SetMusicVolume(currentMusic->music, currentMusic->volume);
        std::cout << "AudioHandlerSystem: Loaded music '" << currentMusic->name << "' from " << currentMusic->pathfile << std::endl;
        return true;
    } else {
        std::cout << "AudioHandlerSystem: Failed to load music '" << currentMusic->name << "' from " << currentMusic->pathfile << std::endl;
        return false;
    }
}

bool AudioHandlerSystem::LoadMusic(const std::string& path, const std::string& name) {
    currentMusic = std::make_unique<MusicSource>();
    currentMusic->name = name;
    currentMusic->pathfile = path;
    return LoadMusic();
}

void AudioHandlerSystem::UnloadSound() {
    if (currentSound) {
        if (currentSound->playing) {
            ::StopSound(currentSound->sound);
            currentSound->playing = false;
        }
        if (currentSound->isLoaded) {
            ::UnloadSound(currentSound->sound);
        }
        std::cout << "AudioHandlerSystem: Unloaded sound '" << currentSound->name << "'" << std::endl;
    }
}

void AudioHandlerSystem::UnloadMusic() {
    if (currentMusic) {
        if (currentMusic->playing) {
            ::StopMusicStream(currentMusic->music);
            currentMusic->playing = false;
        }
        if (currentMusic->isLoaded) {
            ::UnloadMusicStream(currentMusic->music);
        }
        std::cout << "AudioHandlerSystem: Unloaded music '" << currentMusic->name << "'" << std::endl;
    }
}

void AudioHandlerSystem::PlaySound() {
    if (currentSound->isLoaded) {
        ::SetSoundVolume(currentSound->sound, currentSound->volume);
        ::PlaySound(currentSound->sound);
        currentSound->playing = true;
    }
}

void AudioHandlerSystem::PauseSound() {
    if (currentSound->isLoaded) {
        ::PauseSound(currentSound->sound);
    }
}

void AudioHandlerSystem::ResumeSound() {
    if (currentSound->isLoaded) {
        ::ResumeSound(currentSound->sound);
    }
}

void AudioHandlerSystem::PlayMusic() {
    if (currentMusic->isLoaded) {
        ::SetMusicVolume(currentMusic->music, currentMusic->volume);
        ::PlayMusicStream(currentMusic->music);
        currentMusic->playing = true;
    }
}

void AudioHandlerSystem::PauseMusic() {
    if (currentMusic->isLoaded) {
        ::PauseMusicStream(currentMusic->music);
    }
}
void AudioHandlerSystem::ResumeMusic() {
    if (currentMusic->isLoaded) {
        ::ResumeMusicStream(currentMusic->music);
        }
}
