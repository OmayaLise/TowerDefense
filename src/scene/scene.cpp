#include "scene.hpp"

// initialise the state of the game at start
SceneManager::SceneManager()
{
    mState = GameState::START;
}

SceneManager::~SceneManager()
{
}

// draw and update according to the scene called
void SceneManager::Run(Resource &resource)
{
    while (!WindowShouldClose())
    {
        // update music
        resource.music.UpdateMusic();

        BeginDrawing();
        switch (mState)
        {
        case GameState::START:
        {
            mState = mSceneStart.DrawAndUpdateStart(resource);
            // mState = GameState::MENU;
            break;
        }
        case GameState::MENU:
        {
            mSceneGame.GetGame().myMap.ClearMap();
            mState = mSceneMenu.DrawAndUpdateMainMenu(resource);

            // reset game 
            if (mState == GameState::MAPCHOICE || mState == GameState::MAPCREATOR)
            {
                mSceneGame.Reset();
            }

            break;
        }
        case GameState::MAPCHOICE:
        {
            mState = mSceneGame.DrawAndUpdateMapChoice(resource);
            
            //reset game 
            if (mState == GameState::GAME)
                mSceneGame.Reset();
            break;
        }
        case GameState::GAME:
        {
            mState = mSceneGame.DrawAndUpdateGame(resource);
            if (mState == GameState::GAMEOVER)
            {
                mSceneGameOver = {};
            }

            break;
        }
        case GameState::GAMEOVER:
        {
            mState = mSceneGameOver.DrawAndUpdateGameOver(resource, mSceneGame.GetGame());
            break;
        }
        case GameState::PAUSE:
        {
            mState = mScenePause.DrawAndUpdateScenePause(resource);
            break;
        }
        case GameState::SCOREBOARD:
        {
            int value = mSceneScoreboard.UpdateAndDraw(mSceneGame.GetGame(), resource);
            switch (value)
            {
            case 0:
                mState = GameState::SCOREBOARD;
                break;
            case 1:
                mState = GameState::MENU;
                break;
            default:
                mState = GameState::QUIT;
                break;
            }
            break;
        }
        case GameState::MAPCREATOR:
        {
            bool shouldQuit = false;
            if (mSceneGame.GetGame().myMap.MapCreator(resource, shouldQuit))
            {
                mState = GameState::GAME;
            }
            else if (shouldQuit)
            {
                mState = GameState::MENU;
            }
            break;
        }
        default:;
        }

        if (mState == GameState::QUIT)
        {
            break;
        }

        EndDrawing();
    }
}

// update and draw scene start
GameState SceneStart::DrawAndUpdateStart(Resource &resource)
{
    DrawFPS(10, 10);
    ClearBackground(BLACK);
    if (mFrame <= 255)
    {

        unsigned char colorU = static_cast<unsigned char>(mColor); // cast a float to a unsigned char
        DrawTexturePro(resource.startMenu, {80, 93, 870, 783}, {350 + 870 / 2, 50 + 783 / 2, mImagewidth, mImageheight}, {870 / 2, 783 / 2}, mRotate, {colorU, colorU, colorU, 255});
        mColor += 1;
        mRotate = 360 * mColor / 255;
        mImagewidth = 870 * mColor / 255;
        mImageheight = 783 * mColor / 255;
    }
    else
    {
        DrawTexturePro(resource.startMenu, {80, 93, 870, 783}, {350 + 870 / 2, 50 + 783 / 2, 870, 783}, {870 / 2, 783 / 2}, 00, WHITE);
    }
    mFrame ++;
    if (mFrame >= 255 + 30)
    {
        return GameState::MENU;
    }
    else
    {
        return GameState::START;
    }
}

// update and draw main menu
GameState SceneMainMenu::DrawAndUpdateMainMenu(Resource &resource)
{
    ClearBackground(RAYWHITE);
    mMousePos.x = GetMouseX();
    mMousePos.y = GetMouseY();
 
    // launche the music
    resource.music.MyStopMusic(MusicType::GameOverPart2);
    resource.music.MyPlayMusic(MusicType::Menu);

    DrawTexturePro(resource.mainMenu, (Rectangle){0, 0, 600, 400}, (Rectangle){0, 0, constScreenWidth + 200, constScreenHeight}, (Vector2){0, 0}, 0, WHITE);
    DrawText("TOWER", 300, 100, 100, BLACK);
    DrawText("DEFENSE", 250, 200, 100, BLACK);

    // Show slider for sound and music
    DrawTexturePro(resource.playerUI, {456, 62, 352, 143}, {10, 800, 500, 200}, {0, 0}, 0, RAYWHITE);
    resource.music.ShowSliders({10, 800});

    if (ButtonAsset(resource.play, {50, 0, 600, 600}, (constScreenWidth + 200) * 2 / 3 + 85, constScreenHeight * 1 / 2 + 80, 120, 120, LIME, DARKGRAY))
    {
        return GameState::MAPCHOICE;
    }

    if (ButtonAsset(resource.quit, {50, 0, 600, 600}, (constScreenWidth + 200) - 130, 10, 115, 110, RED, DARKGRAY))
    {
        return GameState::QUIT;
    }

    if (ButtonS({(constScreenWidth + 200) - 485, constScreenHeight - 300}, {(constScreenWidth + 200) - 485, constScreenHeight - 280}, 30, 200, 65, "MAP EDITOR", GREEN, DARKGRAY, BLANK, BLANK))
    {
        return GameState::MAPCREATOR;
    }

    return GameState::MENU;
}

// reset value
void SceneGame::Reset()
{
    //Initialize variables
    mGame.Reset();
    mPlayer = Player();
}

GameState SceneGame::DrawAndUpdateMapChoice(Resource &resource)
{
    ClearBackground(BLACK);

    if (mGame.myMap.DrawMapChoice(resource))
    {
        return GameState::GAME;
    }

    return GameState::MAPCHOICE;
}

GameState SceneGame::DrawAndUpdateGame(Resource &resource)
{
    ClearBackground(RAYWHITE);
    mGame.myMap.ShowCadrillage(resource);
    
    mMousePos.x = GetMouseX();
    mMousePos.y = GetMouseY();

    // launche the music
    resource.music.MyStopMusic(MusicType::Menu);
    resource.music.MyPlayMusic(MusicType::Game);

    // draw shop
    DrawFPS(10, 10);
    mGame.DrawShop(resource, mPlayer);
    mGame.DragAndDrop(mGame.towersArray, resource, mPlayer);
    mGame.UpdateTower(mGame.towersArray, mGame.enemiesArray, resource, mPlayer);
    
    // highlight box if cursor in box 
    mGame.myMap.HighlightCurTile();

    // draw player info
    mGame.DrawPlayer(resource, mPlayer);

    // launche the next wave
    if (mPlayer.life > 0)
    {
        if (mGame.WaveCleared())
        {
            if (ButtonAsset(resource.next, {75, 5, 550, 550}, constScreenWidth + 20, constScreenHeight - 75 - 50, 75, 75, LIME, DARKGRAY))
            {
                mGame.Reset(mGame.GetRound() + 1);
            }
        }
    }

    // check life of player 
    if (mPlayer.life <= 0 || mGame.UpdateEnemy(mPlayer, resource))
    {
        //  draw destruction castle
        if (mGame.GenerateExplosion(resource) < 64)
        {
            return GameState::GAME;
        }

        resource.music.MyPlaySound(SoundType::GameOverPart1);
        return GameState::GAMEOVER;
    }

    // call scene pause
    if (ButtonAsset(resource.pause, {0, 0, 512, 512}, constScreenWidth + (125 / 2) + 45, constScreenHeight - 75 - 50, 75, 75, {253, 249, 0, 200}, DARKGRAY))
    {
        PauseMusicStream(resource.music.musicUsed[1]);
        return GameState::PAUSE;
    }

    return GameState::GAME;
}

// update and draw Scene pause
GameState ScenePause::DrawAndUpdateScenePause(Resource &resource)
{
    mMousePos.x = GetMouseX();
    mMousePos.y = GetMouseY();

    DrawText("Pause", constScreenWidth / 2 - 130, constScreenHeight / 3 - 200, 100, BLACK);

    // Show slider for sound and music
    DrawTexturePro(resource.playerUI, {456, 62, 352, 143}, {constScreenWidth / 2 - 210, 700, 500, 200}, {0, 0}, 0, RAYWHITE);
    resource.music.ShowSliders({constScreenWidth / 2 - 210, 700});

    // draw button resume
    if (ButtonAsset(resource.resume, {120, 50, 460, 460}, constScreenWidth / 2 - 32, constScreenHeight / 4 + 100, 150, 150, YELLOW, GRAY))
    {
        ResumeMusicStream(resource.music.musicUsed[1]);
        return GameState::GAME;
    }

    // draw button menu
    if (ButtonAsset(resource.home, {48, 30, 602, 538}, constScreenWidth / 3, constScreenHeight / 2, 150, 150, WHITE, GRAY))
    {
        resource.music.MyStopMusic(MusicType::Game);
        resource.music.MyPlayMusic(MusicType::Menu);
        return GameState::MENU;
    }

    // draw button quit
    if (ButtonAsset(resource.quit, {50, 0, 600, 600}, constScreenWidth * 3 / 5, constScreenHeight / 2, 150, 150, RED, GRAY))
    {
        return GameState::QUIT;
    }

    return GameState::PAUSE;
}

// update and draw game over
GameState SceneGameOver::DrawAndUpdateGameOver(Resource &resource, const Game &lastGame)
{
    ClearBackground(BLACK);
    mMousePos.x = GetMouseX();
    mMousePos.y = GetMouseY();
 
    StopMusicStream(resource.music.musicUsed[1]);
 
    DrawText("GAME OVER", (constScreenWidth + 200) / 2 - 250, 100, 80, WHITE);
    if (mFrame <= 255)
    {
        unsigned char colorU = static_cast<unsigned char>(mFrame); //cast float to unsigned char
        DrawTexturePro(resource.gameOver, {0, 0, 600, 600}, {(constScreenWidth + 200) / 2 - 200, 250, 400, 400}, {0, 0}, 0, {colorU, colorU, colorU, 255});
        mFrame ++;
    }
    else
    {
        // launche the music
        resource.music.musicUsed[2].looping = true;
        PlayMusicStream(resource.music.musicUsed[2]);

        unsigned char colorU = static_cast<unsigned char>(mColor); //cast float to unsigned char
        DrawTexturePro(resource.home, {48, 30, 602, 538}, {(constScreenWidth + 200) * 0.5 - 350 - 150, 250 + 150, 150, 150}, {0, 0}, 0, {colorU, colorU, colorU, 255});
        DrawTexturePro(resource.quit, {50, 0, 600, 600}, {(constScreenWidth + 200) * 0.5 + 200 + 140, 250 + 150, 150, 150}, {0, 0}, 0, {colorU, colorU, colorU, 255});
        DrawText("Save Score", constScreenWidth * 2 / 6 + 70, constScreenHeight * 4 / 5, 80, {colorU, colorU, colorU, 255});
        mColor += 4;

        DrawTexturePro(resource.gameOver, {0, 0, 600, 600}, {(constScreenWidth + 200) / 2 - 200, 250, 400, 400}, {0, 0}, 0, WHITE);
        DrawText(TextFormat("Round : %d", lastGame.GetRound()), (constScreenWidth + 200) / 2 - 200 - 20, 250 + 475, 80, WHITE);

        if (InRec(mMousePos.x, mMousePos.y, (constScreenWidth + 200) * 0.5 - 350 - 150, 250 + 150, 150, 150) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return GameState::MENU;
        }
        if (InRec(mMousePos.x, mMousePos.y, (constScreenWidth + 200) * 0.5 + 200 + 140, 250 + 150, 150, 150) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return GameState::QUIT;
        }
        if (InRec(mMousePos.x, mMousePos.y, constScreenWidth * 2 / 6 + 70, constScreenHeight * 4 / 5, 472, 75) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            return GameState::SCOREBOARD;
        }
    }
    return GameState::GAMEOVER;
}