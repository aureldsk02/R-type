#pragma once
#include <iostream>

class GameSettings {
    public:
        enum class GameMode {
            NORMAL,
            PVP,
            CUSTOM
        };
    
        enum class Difficulty {
            EASY,
            MEDIUM,
            DIFFICULT
        };
    
        GameSettings(GameMode mode = GameMode::NORMAL,
                     Difficulty difficulty = Difficulty::EASY,
                     const std::string& filePath = "")
            : _mode(mode), _difficulty(difficulty), _filePath(filePath) {}
    
        GameMode getMode() const { return _mode; }
        Difficulty getDifficulty() const { return _difficulty; }
        const std::string& getFilePath() const { return _filePath; }
    
        void setMode(GameMode mode) { _mode = mode; }
        void setDifficulty(Difficulty diff) { _difficulty = diff; }
        void setFilePath(const std::string& path) { _filePath = path; }
    
        static GameMode modeFromString(const std::string& str) {
            if (str == "PVP") return GameMode::PVP;
            if (str == "CUSTOM") return GameMode::CUSTOM;
            return GameMode::NORMAL;
        }
    
        static Difficulty difficultyFromString(const std::string& str) {
            if (str == "MEDIUM") return Difficulty::MEDIUM;
            if (str == "DIFFICULT") return Difficulty::DIFFICULT;
            return Difficulty::EASY;
        }
    
        static std::string modeToString(GameMode mode) {
            switch (mode) {
                case GameMode::PVP: return "PVP";
                case GameMode::CUSTOM: return "CUSTOM";
                default: return "NORMAL";
            }
        }
    
        static std::string difficultyToString(Difficulty diff) {
            switch (diff) {
                case Difficulty::MEDIUM: return "MEDIUM";
                case Difficulty::DIFFICULT: return "DIFFICULT";
                default: return "EASY";
            }
        }
    private:
        GameMode _mode;
        Difficulty _difficulty;
        std::string _filePath;
};