/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Audio Handler System
*/

#ifndef AUDIO_HANDLER_HPP_
#define AUDIO_HANDLER_HPP_

#include <raylib.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

// Audio resource structures
struct SoundSource {
    Sound sound;
    std::string name;
    std::string pathfile;
    bool isLoaded = false;
    float volume = 1.0f;
    bool playing = false;

    SoundSource() : sound{}, name(""), pathfile(""), isLoaded(false), volume(1.0f), playing(false) {}
};

struct MusicSource {
    Music music;
    std::string name;
    std::string pathfile;
    bool isLoaded = false;
    float volume = 1.0f;
    bool loop = true;
    bool playing = false;

    MusicSource() : music{}, name(""), pathfile(""), isLoaded(false), volume(1.0f), loop(true), playing(false) {}
};

// ECS Audio Handler System
class AudioHandlerSystem {
    private:
        // std::unordered_map<std::string, std::unique_ptr<SoundSource>> sounds;
        // std::unordered_map<std::string, std::unique_ptr<MusicSource>> musics;
        std::unique_ptr<MusicSource> currentMusic;
        std::unique_ptr<SoundSource> currentSound; // Track currently playing sound (optional, for single sound playback)
        bool initialized = false;
        bool audioInitialized = false;

    public:
        AudioHandlerSystem() : currentMusic(nullptr), currentSound(nullptr), initialized(false) {}
        AudioHandlerSystem(MusicSource& music, SoundSource& sound)
            : currentMusic(std::make_unique<MusicSource>(music)),
              currentSound(std::make_unique<SoundSource>(sound)),
              initialized(false) {
        Initialize();
              }
        ~AudioHandlerSystem();

        void play_sound();
        void play_music();

        // System lifecycle
        void Initialize();
        void Update();
        void Update_currentMusic(MusicSource& music);
        void Update_currentSound(SoundSource& sound);
        void Cleanup();
        bool IsInitialized() const { return initialized; }

        // Audio management
        bool LoadSound();
        bool LoadMusic();
        bool LoadMusic(const std::string& path, const std::string& name);
        void UnloadSound();
        void UnloadMusic();

        // Sound playback
        void PlaySound();
        void PauseSound();
        void ResumeSound();

        // Music playback
        void PlayMusic();
        void PauseMusic();
        void ResumeMusic();

        // Utility functions
        std::string GetCurrentMusic() const { return currentMusic.get()->name; }
        std::string GetCurrentSound() const { return currentSound.get()->name; }
};

#endif /* !AUDIO_HANDLER_HPP_ */