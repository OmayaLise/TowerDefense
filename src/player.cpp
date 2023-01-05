#include "player.hpp"


//Constructor
Player::Player()
{
    lifeMax = 100;
    life = 100;
    money = 250;
}

//Destructor
Player::~Player()
{

}


//Player loose life and GameOver if life goes to 0
void Player::LooseLife(const int degats)
{
   life -= degats;
   if(life < 0)
        life = 0;
}



