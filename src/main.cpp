#include <raylib.h>

#include "scene/scene.hpp"


int main(void)
{
    const int screenWidth = constScreenWidth + 200;
    const int screenHeight = constScreenHeight;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Tower Defense");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(0);

    {
        Resource resource;
        SceneManager scene;
        scene.Run(resource);
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
