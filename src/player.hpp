#pragma once
#include <raylib.h>
#include "setup.hpp"

//Define class Player
class Player
{
public:
    int life;
    int lifeMax;
    int money;

    //Constructor
    Player();

    //Destructor
    ~Player();

    //Player loose life and GameOver if life goes to 0
    void LooseLife(int degats);

};