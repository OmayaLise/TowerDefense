#pragma once
#include <raylib.h>
#include "../include/math_towerdefense.hpp"
#include "soundAndMusic.hpp"

constexpr int constScreenWidth = 1408;
constexpr int constScreenHeight = 1024;
constexpr int constSizeBox = 64;

constexpr int constBoxRow = constScreenHeight / constSizeBox;
constexpr int constBoxColumn = constScreenWidth / constSizeBox;
constexpr int constNumberMaxBox = constBoxRow * constBoxColumn;

constexpr int radToDeg = 180 / PI;

// Towers prices to access in menu without creating tower
constexpr int constPriceClassicT = 80;
constexpr int constPriceSlowingT = 300;
constexpr int constPriceExplosiveT = 500;
constexpr int constPriceSniperT = 400;

// Need to be change if assets are changed
constexpr int constTileSheetWidth = 23;
constexpr int constTileSheetHeight = 13;

class Resource
{
private:

public:
    Texture2D tilesheet;
    Texture2D mainMenu;
    Texture2D play;
    Texture2D quit;
    Texture2D pause;
    Texture2D backgroundMenu;
    Texture2D gameOver;
    Texture2D home;
    Texture2D startMenu;
    Texture2D next;
    Texture2D explosion;
    Texture2D resume;
    Texture2D hammer;
    Texture2D tpParticle;
    Texture2D playerUI;
    Texture2D mapOnePreview;
    Texture2D mapTwoPreview;

    SoundAndMusic music;

    Resource()
    {
        tilesheet = LoadTexture("assets/texture/towerDefense_tilesheet.png");
        mainMenu = LoadTexture("assets/texture/menu.png");
        play = LoadTexture("assets/texture/play.png");
        quit = LoadTexture("assets/texture/exit.png");
        pause = LoadTexture("assets/texture/buttonPause.png");
        backgroundMenu = LoadTexture("assets/texture/background.png");
        gameOver = LoadTexture("assets/texture/gameover.png");
        home = LoadTexture("assets/texture/logoHome.png");
        startMenu = LoadTexture("assets/texture/montd'or.png");
        next = LoadTexture("assets/texture/next.png");
        explosion = LoadTexture("assets/texture/Explosion.png");
        resume = LoadTexture("assets/texture/resume.png");
        hammer = LoadTexture("assets/texture/genericItem_color_023.png");
        tpParticle = LoadTexture("assets/texture/Enderman_particles.png");
        playerUI = LoadTexture("assets/texture/bouttons.png");
        mapOnePreview = LoadTexture("assets/texture/map_one_preview.png");
        mapTwoPreview = LoadTexture("assets/texture/map_two_preview.png");
    }

    ~Resource()
    {
        UnloadTexture(tilesheet);
        UnloadTexture(mainMenu);
        UnloadTexture(play);
        UnloadTexture(quit);
        UnloadTexture(pause);
        UnloadTexture(backgroundMenu);
        UnloadTexture(gameOver);
        UnloadTexture(home);
        UnloadTexture(startMenu);
        UnloadTexture(next);
        UnloadTexture(explosion);
        UnloadTexture(resume);
        UnloadTexture(hammer);
        UnloadTexture(tpParticle);
        UnloadTexture(playerUI);
        UnloadTexture(mapOnePreview);
        UnloadTexture(mapTwoPreview);
    }

    Resource(const Resource& copy) = delete;
    Resource& operator=(const Resource& copy) = delete;
};

inline bool ButtonS(vector2D positionRec, vector2D positionTxt, int fontSize, float width, float height, const char *info, Color preColor, Color nextColor, Color recColor , Color hoverColor)
{
    
    if (InRec(GetMouseX(), GetMouseY(), positionRec.x, positionRec.y, width, height))
    {
        DrawRectangle(positionRec.x, positionRec.y, width, height, hoverColor);
        DrawText(TextFormat("%s", info), positionTxt.x, positionTxt.y, fontSize, preColor);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            DrawRectangle(positionRec.x, positionRec.y, width, height, GRAY);
            return true;
        }
    }
    else
    {
        DrawRectangle(positionRec.x, positionRec.y, width, height, recColor);
        DrawText(TextFormat("%s", info), positionTxt.x, positionTxt.y, fontSize, nextColor);
    }
    return false;
}

inline bool ButtonAsset(Texture2D asset, Rectangle source, float x, float y, float width, float height, Color colorInRec, Color colorOutRec)
{
    if (InRec(GetMouseX(), GetMouseY(), x, y, width, height))
    {
        DrawTexturePro(asset, source, (Rectangle){x, y, width, height}, (Vector2){0, 0}, 0, colorInRec);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return true;
        }
    }
    else
    {
        DrawTexturePro(asset, source, (Rectangle){x, y, width, height}, (Vector2){0, 0}, 0, colorOutRec);
    }
    return false;
}