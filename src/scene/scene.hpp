#pragma once
#include <raylib.h>

#include "../game.hpp"
#include "../scoreboard.hpp"

enum class GameState
{
    START,
    RESUME,
    MENU,
    PAUSE,
    GAMEOVER,
    GAME,
    QUIT,
    SCOREBOARD,
    MAPCREATOR,
    MAPCHOICE,
};

 // Define virtual class Scene

//First Screen 
class SceneStart 
{
private:
    float mFrame = 0;
    float mRotate = 0;
    float mColor = 0;
    float mImagewidth = 0;
    float mImageheight = 0;

public:
    SceneStart(){};
    ~SceneStart(){};
    GameState DrawAndUpdateStart(Resource &resource);
};

// class draw and update mSceneMenu
class SceneMainMenu
{
private:
    vector2D mMousePos;
public:
    SceneMainMenu(){};
    ~SceneMainMenu(){};
    GameState DrawAndUpdateMainMenu(Resource &resource);
};

class SceneGame
{
private:


    //Initialize variables
    Game mGame;
    Player mPlayer;
    vector2D mMousePos;

public:
    SceneGame(){}

    SceneGame(const SceneGame& copy) = delete;
    SceneGame& operator=(const SceneGame& copy) = delete;

    void Reset();
    
    GameState DrawAndUpdateMapChoice(Resource& resource);
    GameState DrawAndUpdateGame(Resource &resource);

    Game& GetGame() { return mGame; }
};

class ScenePause
{
private:
vector2D mMousePos;
public:
    ScenePause(){};
    ~ScenePause(){};
    GameState DrawAndUpdateScenePause(Resource &resource);
};

class SceneGameOver
{
private:
    vector2D mMousePos;
    float mFrame = 0;
    int mColor = 0;
public:
    SceneGameOver(){};
    ~SceneGameOver(){};
    GameState DrawAndUpdateGameOver(Resource &resource, const Game& lastGame);
};


class SceneManager
{
private:
    GameState mState = GameState::START;
    SceneStart mSceneStart;
    SceneMainMenu mSceneMenu;
    SceneGame mSceneGame;
    ScenePause mScenePause;
    SceneGameOver mSceneGameOver;
    Scoreboard mSceneScoreboard;
public:
    SceneManager();
    ~SceneManager();

    void Run(Resource &resource);
};