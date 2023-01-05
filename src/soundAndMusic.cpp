#include "soundAndMusic.hpp"

SoundAndMusic::SoundAndMusic()
{
    // load texture for sound and music
    mSoundOff = LoadTexture("assets/texture/muteSound.png");
    mSoundOn = LoadTexture("assets/texture/Sound.png");
    mMusicOff = LoadTexture("assets/texture/muteMusic.png");
    mMusicOn = LoadTexture("assets/texture/Music.png");

    // load music
    musicUsed[0] = LoadMusicStream("assets/soundAndMusic/soundMenu.mp3");
    musicUsed[1] = LoadMusicStream("assets/soundAndMusic/soundGame.mp3");
    musicUsed[2] = LoadMusicStream("assets/soundAndMusic/GameOver2.mp3");

    // load sound
    soundUsed[0] = LoadSound("assets/soundAndMusic/soundExplosion.mp3");
    soundUsed[1] = LoadSound("assets/soundAndMusic/laser.wav");
    soundUsed[2] = LoadSound("assets/soundAndMusic/foom_0.wav");
    soundUsed[3] = LoadSound("assets/soundAndMusic/sniperSound.mp3");
    soundUsed[4] = LoadSound("assets/soundAndMusic/classique.mp3");
    soundUsed[5] = LoadSound("assets/soundAndMusic/healer.mp3");
    soundUsed[6] = LoadSound("assets/soundAndMusic/strong.mp3");
    soundUsed[7] = LoadSound("assets/soundAndMusic/bossDied.mp3");
    soundUsed[8] = LoadSound("assets/soundAndMusic/EndermanTeleportSound.mp3");
    soundUsed[9] = LoadSound("assets/soundAndMusic/YOU DIED.mp3");
}

void SoundAndMusic::MyPlaySound(SoundType sound)
{
    PlaySound(soundUsed[(int)sound]);
}

void SoundAndMusic::MyPlayMusic(MusicType music)
{
    PlayMusicStream(musicUsed[(int)music]);
}

void SoundAndMusic::MyStopMusic(MusicType music)
{
    StopMusicStream(musicUsed[(int)music]);
}
// set a volume for all music
void SoundAndMusic::SetMusicVolume(float volume)
{
    mMusicVolume = volume;
    for (size_t i = 0; i < musicUsed.size(); ++i)
        ::SetMusicVolume(musicUsed[i], mMusicVolume);
}

// set a volume for all sound
void SoundAndMusic::SetSoundVolume(float volume)
{
    mSoundVolume = volume;
    for (size_t i = 0; i < soundUsed.size(); ++i)
        ::SetSoundVolume(soundUsed[i], mSoundVolume);
}

// update all music
void SoundAndMusic::UpdateMusic()
{
    for (size_t i = 0; i < musicUsed.size(); i++)
    {
        UpdateMusicStream(musicUsed[i]);
        musicUsed[i].looping = true;
    }
}

// draw toggle button
bool SoundAndMusic::SettingsToggleButton(bool *toggle, Texture2D on, Texture2D off, vector2D buttonPos)
{
    Texture2D tex;
    if (*toggle)
        tex = off;
    else
        tex = on;

    Rectangle buttonRect = {buttonPos.x, buttonPos.y, 50, 50};

    DrawTexturePro(tex, {0, 0, 532, 500}, buttonRect, {0, 0}, 0, {117, 91, 77, 225});

    vector2D mousePos = {(float)GetMouseX(), (float)GetMouseY()};
    if (InRec(mousePos.x, mousePos.y, buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *toggle = !*toggle;
            return true;
        }
    }

    return false;
}

// draw slider according to the volume
bool SoundAndMusic::SettingsSlider(float *value, vector2D pos, bool *toggle)
{
    float width = 300;
    DrawLine(pos.x, pos.y, pos.x + width, pos.y, GRAY);
    DrawCircle(pos.x, pos.y, 6, BLACK);
    DrawCircle(pos.x + width, pos.y, 6, BLACK);

    float cursorX = pos.x + (*value) * width;
    DrawCircle(cursorX, pos.y, 6, GREEN);

    Rectangle sliderRect = {pos.x - 1, pos.y, width + 2, 10.f};
    vector2D mousePos = {(float)GetMouseX(), (float)GetMouseY()};
    if (InRec(mousePos.x, mousePos.y, sliderRect.x, sliderRect.y, sliderRect.width, sliderRect.height) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        *value = -(pos.x - mousePos.x) / width;

        // change toggle button if voulume = 0
        if (*value == 0)
        {
            *toggle = false;
        }
        else
        {
            *toggle = true;
        }

        return true;
    }
    return false;
}

void SoundAndMusic::ShowSliders(vector2D settingPos)
{
    // Draw sound slider and toggle button
    {

        if (SettingsToggleButton(&mIsSoundOn, mSoundOff, mSoundOn, {settingPos.x + 60, settingPos.y + 100}))
            SetSoundVolume(mIsSoundOn ? 0.5f : 0.f);

        float vol = mSoundVolume;
        if (SettingsSlider(&vol, {settingPos.x + 140, settingPos.y + 130}, &mIsSoundOn))
            SetSoundVolume(vol);
    }

    // Draw music slider and toggle button
    {
        if (SettingsToggleButton(&mIsMusicOn, mMusicOff, mMusicOn, {settingPos.x + 55, settingPos.y + 40}))
            SetMusicVolume(mIsMusicOn ? 0.5f : 0.f);

        float vol = mMusicVolume;
        if (SettingsSlider(&vol, {settingPos.x + 140, settingPos.y + 60}, &mIsMusicOn))
            SetMusicVolume(vol);
    }
}

SoundAndMusic::~SoundAndMusic()
{
    // unload texture
    UnloadTexture(mSoundOff);
    UnloadTexture(mSoundOn);
    UnloadTexture(mMusicOn);
    UnloadTexture(mMusicOff);

    // unload music
    for (int i = 0; i < 3; i++)
    {
        UnloadMusicStream(musicUsed[i]);
    }

    // unload sound
    for (int i = 0; i < 10; i++)
    {
        UnloadSound(soundUsed[i]);
    }
}
