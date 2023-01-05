#pragma once
#include <raylib.h>
#include "../src/entities/enemies/enemies.hpp"
#include "../src/entities/towers/towers.hpp"
#include "player.hpp"

extern int nbRound;

// Define class game
class Game
{
private:
    int mCurrentRound = 0;
    float mTimeCounter = 0;
    size_t mNbEnemySpawn = 0;
    float mEnemySpawnRate = 0;
    int mAnimIndexExplosion = 0;

    void GenerateEnemy(int nbRound);

public:
    Map myMap;
    std::vector<Enemies *> enemiesArray;
    std::vector<Towers *> towersArray;

    //Constructor
    Game(int nbRound = 0);

    Game(const Game &copy) = delete;
    Game &operator=(const Game &copy) = delete;

    void Reset(int nbRound = 0);

    //Destructor
    ~Game();

    //Function called if player goes to 0 hp
    bool GameOver(Player &player, Resource& resource);
    void SetEnemySpawnRate();
    bool MoveEnemy(Player &player, Enemies *enemy, Resource& resource);
    bool UpdateEnemy(Player &player, Resource& resource);
    bool WaveCleared();
    void DragAndDrop(std::vector<Towers *> &towersArray, Resource &resource, Player &player);
    void DrawDragTower (int sTowerType, Resource &resource);
    void UpdateTower(std::vector<Towers *> &towersArray, std::vector<Enemies *> &enemiesArray, Resource &resource, Player &player);
    void DrawShop(Resource &resource, Player &player);
    void DrawPlayer(Resource &resource, Player &player);
    void DrawLineOnMap(Resource &resource);
    int GetRound() const { return mCurrentRound; }

    int GenerateExplosion(Resource& resource);
};