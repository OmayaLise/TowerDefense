#pragma once
#include <array>
#include "raylib.h"
#include "../include/math_towerdefense.hpp"

enum class SoundType
{
    ClassicShoot = 0,
    SlowShoot = 1,
    ExplosiveShoot = 2,
    SniperShoot = 3,
    DiedClassic = 4,
    DiedHealer = 5,
    DiedStrong = 6,
    DiedTank = 7,
    TeleportSound = 8,
    GameOverPart1 = 9,
};

enum class MusicType
{
    Menu = 0,
    Game = 1,
    GameOverPart2 = 2,
};

class SoundAndMusic
{
private:
    float mMusicVolume = 0.5f;
    float mSoundVolume = 0.5f;
    bool mIsSoundOn = true;
    bool mIsMusicOn = true;
    Texture2D mSoundOff;
    Texture2D mSoundOn;
    Texture2D mMusicOff;
    Texture2D mMusicOn;

    bool SettingsToggleButton(bool *toggle, Texture2D on, Texture2D off, vector2D buttonPos);
    bool SettingsSlider(float *value, vector2D pos, bool *toggle);
    void SetMusicVolume(float volume);
    void SetSoundVolume(float volume);

public:
    std::array<Music, 3> musicUsed;

    std::array<Sound, 10> soundUsed;

    SoundAndMusic();
    ~SoundAndMusic();

    void MyPlaySound(SoundType sound);
    void MyPlayMusic(MusicType music);
    void MyStopMusic(MusicType music);
    

    
    void UpdateMusic();
    void ShowSliders(vector2D settingsPos);
};

/*
class SoundAndMusicSettings
{
public:
    Texture2D mSoundOff;
    Texture2D mSoundOn;
    Texture2D mMusicOff;
    Texture2D mMusicOn;

    void Show(SoundAndMusic& soundMusic, vector2D settingsPos);
};
*/