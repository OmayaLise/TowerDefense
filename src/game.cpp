#include "game.hpp"

//Constructor
Game::Game(int nbRound)
{
    mCurrentRound = nbRound;
    //Set the spawn rate of enemies
    SetEnemySpawnRate();

    //Generate the list of enemy and set the number of enemy to spawn
    GenerateEnemy(mCurrentRound);
}

//Destructor
Game::~Game()
{

    Reset();
}

void Game::Reset(int nbRound)
{
    //Reset all values
    mCurrentRound = nbRound;

    mTimeCounter = 0;
    mNbEnemySpawn = 0;
    mEnemySpawnRate = 0;
    mAnimIndexExplosion = 0;

    //Clear vectors
    for (Enemies *enemy : enemiesArray)
        delete enemy;
    enemiesArray.clear();

    if (mCurrentRound <= 0)
    {
        myMap.Reset(myMap.GetIdMap());
        for (Towers *tower : towersArray)
            delete tower;
        towersArray.clear();
    }

    //Set new vectors
    SetEnemySpawnRate();
    GenerateEnemy(mCurrentRound);
}

//Function called if player goes to 0 hp
bool Game::GameOver(Player &player, Resource &resource)
{
    //If player life is at 0 it's GAME OVER
    if (player.life <= 0)
    {
        for (Enemies *enemy : enemiesArray)
            delete enemy;
        enemiesArray.clear();
        return true;
    }
    else
    {
        return false;
    }
}

//Update the enemy -> Move and deal damage if needed
bool Game::MoveEnemy(Player &player, Enemies *enemy, Resource &resource)
{
    //Function Move return true if the enemy crossed the castle
    if (enemy->Move(myMap, resource))
    {
        //If the enemy walked trough the castle the player take damage
        player.LooseLife(enemy->DoDamage());

        //Go trough the vector to remove from the list the enemy
        for (auto it = enemiesArray.begin(); it != enemiesArray.end(); it++)
        {
            if (*it == enemy)
            {
                enemiesArray.erase(it);
                delete enemy;
                mNbEnemySpawn -= 1;

                break;
            }
        }
    }
    return GameOver(player, resource);
}

//Update enemy position and Create enemy if not initialized
bool Game::UpdateEnemy(Player &player, Resource &resource)
{
    bool isGameOver = false;

    //Increment counter
    mTimeCounter += GetFrameTime();

    //Check if the counter has the value we want and if we didn't already spawned all enemies
    if (mTimeCounter >= mEnemySpawnRate && mNbEnemySpawn < enemiesArray.size())
    {
        //Set the enemy to init so he exist
        enemiesArray[mNbEnemySpawn]->isInit = true;
        //Increment counter of enemy spawn and reset counter mTimeCounter
        mNbEnemySpawn += 1;
        mTimeCounter = 0;
    }

    //Loop on all enemy to draw and make then move
    for (size_t i = 0; i < enemiesArray.size(); i++)
    {
        if (enemiesArray[i]->isInit)
        {
            //Check if life of enemy is at 0 or lower if we yes kill it
            if (enemiesArray[i]->life <= 0)
            {
                Enemies *enemy = enemiesArray[i];

                if (enemy->timeDeadAnimation <= 0.1)
                {
                    DrawTextureRec(resource.explosion, {5 * 32, 0, 32, 32}, {enemy->position.x - 16, enemy->position.y - 16}, WHITE);
                    enemy->timeDeadAnimation += GetFrameTime();
                }
                else if (enemy->timeDeadAnimation <= 0.2)
                {
                    DrawTextureRec(resource.explosion, {6 * 32, 0, 32, 32}, {enemy->position.x - 16, enemy->position.y - 16}, WHITE);
                    enemy->timeDeadAnimation += GetFrameTime();
                }
                else if (enemy->timeDeadAnimation <= 0.3)
                {
                    DrawTextureRec(resource.explosion, {7 * 32, 0, 32, 32}, {enemy->position.x - 16, enemy->position.y - 16}, WHITE);
                    enemy->timeDeadAnimation += GetFrameTime();
                }
                else
                {
                    switch (enemiesArray[i]->GetClass())
                    {
                    case classEnemy::E_CLASSIC:
                        resource.music.MyPlaySound(SoundType::DiedClassic);
                        break;
                    case classEnemy::E_HEAL:
                        resource.music.MyPlaySound(SoundType::DiedHealer);
                        break;
                    case classEnemy::E_STRONG:
                        resource.music.MyPlaySound(SoundType::DiedStrong);
                        break;
                    case classEnemy::E_BOSS:
                        resource.music.MyPlaySound(SoundType::DiedTank);
                        break;
                    default:
                        break;
                    }
                    player.money += enemy->drop;
                    enemiesArray.erase(enemiesArray.begin() + i);
                    delete enemy;
                    i--;
                    // Security to make spawn enemy even if other Enemies are killed
                    mNbEnemySpawn--;
                }
                continue;
            }

            enemiesArray[i]->Draw(resource);
            switch (enemiesArray[i]->GetClass())
            {
            case classEnemy::E_HEAL:
                enemiesArray[i]->Heal(enemiesArray);
            default:
                break;
            }

            if (!isGameOver)
                isGameOver = MoveEnemy(player, enemiesArray[i], resource);
            else
                MoveEnemy(player, enemiesArray[i], resource);
        }
    }

    return isGameOver;
}

void Game::GenerateEnemy( int nbRound)
{
    //Set the limit of healer for each round
    int const healerLimit = nbRound - 1;
    //Set the limit of strong and classic ennemies as FighterLimit
    int const FighterLimit = nbRound + 1;
    //Set the limit of teleport
    int const teleportLimit = nbRound / 5;

    //Set variables to check if a new enemy doesn't reach the limit
    int nbClassic = 0, nbHealer = 0, nbStrong = 0, nbTeleport = 0;
    bool isNotValid = true;

    //We set the number of Enemies for each round at 3 times the number of round
    for (int i = 0; i < nbRound * 3; i++)
    {
        //Generate a random number
        int random = rand() % 4;

        //Put isNotvalid at true to check again
        isNotValid = true;
        //Check if there is not too much enemy of the same type
        while (isNotValid)
        {

            switch (random)
            {
            //Case 0 is for classic enemies
            case 0:
            {
                //Verification
                if (nbClassic == FighterLimit)
                {
                    //Generate a new Random value with only other enemy
                    random = 1 + rand() % 3;
                    break;
                }
                else
                {
                    //Stop the loop
                    isNotValid = false;
                    ClassicEnemies *clasic = new ClassicEnemies(myMap.startPosition, mCurrentRound);
                    enemiesArray.push_back(clasic);
                    nbClassic++;
                    break;
                }
            }
            //Case 1 is for Strong enemies
            case 1:
            {
                //Verification
                if (nbStrong == FighterLimit)
                {
                    //Generate a new Random value
                    random = rand() % 4;
                    break;
                }
                else
                {
                    //Stop the loop
                    isNotValid = false;
                    StrongEnemies *strong = new StrongEnemies(myMap.startPosition, mCurrentRound);
                    enemiesArray.push_back(strong);
                    nbStrong++;
                    break;
                }
            }
            //Case 2 is for Healing enemies
            case 2:
            {
                //Verification
                if (nbHealer == healerLimit)
                {
                    //Generate a new Random value
                    random = rand() % 4;
                    break;
                }
                else
                {
                    //Stop the loop
                    isNotValid = false;
                    HealingEnemies *healer = new HealingEnemies(myMap.startPosition, mCurrentRound);
                    enemiesArray.push_back(healer);
                    nbHealer++;
                    break;
                }
            }
            case 3:
            {
                //Verification
                if (nbTeleport == teleportLimit)
                {
                    //Generate a new Random value with only other enemy
                    random = rand() % 3;
                    break;
                }
                else
                {
                    //Stop the loop
                    isNotValid = false;
                    TeleportEnemy *teleport = new TeleportEnemy(myMap.startPosition, mCurrentRound);
                    enemiesArray.push_back(teleport);
                    nbTeleport++;
                    break;
                }
            }
            }
        }
    }
    //Add boss every 10 round
    if (mCurrentRound % 10 == 0 && mCurrentRound != 0)
    {
        BossEnemies *Boss = new BossEnemies(myMap.startPosition, mCurrentRound);
        enemiesArray.push_back(Boss);
    }
}

//Function to know if there is still enemy alive
bool Game::WaveCleared()
{
    return enemiesArray.empty();
}

//Set the enemy spawn rate to make the game run smoothly
void Game::SetEnemySpawnRate()
{
    //Set a multiplicator to increase spawn speed as round go up
    int mult = mCurrentRound / 5;
    //Set the spawn rate at 1/3 a second and soustract mult
    mEnemySpawnRate = 20 - mult;
    //Security to have a minimal value
    if (mEnemySpawnRate <= 0)
        mEnemySpawnRate = 1;
}

void Game::DragAndDrop(std::vector<Towers *> &towersArray, Resource &resource, Player &player)
{
    // Reused values for drawing
    vector2D txtPosition = {constScreenWidth + 30, constScreenHeight / 10 + constSizeBox * 9};
    float middleShopBox = constScreenWidth + 70;
    float heightScaleShop = constScreenHeight / 10;
    int fontSize = 18;

    static int sTowerType = -1;
    bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    // Check if the mouse is over a button, if it is : set tower type + gives info on towers
    if ((ButtonS({middleShopBox, heightScaleShop - constSizeBox}, txtPosition, fontSize, constSizeBox, constSizeBox, "Classic Tower\nDamage: Medium \nRange: Medium\nSpeed: Medium\nEffect: None\n", GOLD, BLANK, BLANK, BLANK)) && sTowerType == -1 && isMouseDown == true && player.money >= constPriceClassicT)
    {
        sTowerType = 0;
    }
    if ((ButtonS({middleShopBox, heightScaleShop + constSizeBox}, txtPosition, fontSize, constSizeBox, constSizeBox, "Slowing Tower\nDamage: Low \nRange: Small\nSpeed: Fast\nEffect: Slow\n", GOLD, BLANK, BLANK, BLANK)) && sTowerType == -1 && isMouseDown == true && player.money >= constPriceSlowingT)
    {
        sTowerType = 1;
    }
    if ((ButtonS({middleShopBox, heightScaleShop + constSizeBox * 5}, txtPosition, fontSize, constSizeBox, constSizeBox, "Explosive Tower\nDamage: Heavy \nRange: Small\nSpeed: Slow\nEffect: AOE\n", GOLD, BLANK, BLANK, BLANK)) && sTowerType == -1 && isMouseDown == true && player.money >= constPriceExplosiveT)
    {
        sTowerType = 2;
    }
    if ((ButtonS({middleShopBox, heightScaleShop + constSizeBox * 7}, txtPosition, fontSize, constSizeBox, constSizeBox, "Destroy tower\nGet half of its \nprice back", GOLD, BLANK, BLANK, BLANK)) && sTowerType == -1 && isMouseDown == true)
    {
        sTowerType = 3;
    }
    if ((ButtonS({middleShopBox, heightScaleShop + constSizeBox * 3}, txtPosition, fontSize, constSizeBox, constSizeBox, "Sniper Tower\nDamage: Heavy \nRange: Infinite\nSpeed: Slow\nEffect: Target \nanywhere on map", GOLD, BLANK, BLANK, BLANK)) && sTowerType == -1 && isMouseDown == true && player.money >= constPriceSniperT)
    {
        sTowerType = 4;
    }
    // Get the right texture to drag
    if (sTowerType >= 0)
    {
        DrawLineOnMap(resource);
        DrawDragTower(sTowerType, resource);
    }
    // Create right tower
    if (sTowerType >= 0 && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        int index = myMap.FindBox(GetMousePosition().x, GetMousePosition().y);

        if (myMap.mTabBox[index].isFree && index != -1)
        {
            Towers *tower = nullptr;
            switch (sTowerType)
            {
            case 0:
                if (player.money >= constPriceClassicT)
                {
                    tower = new ClassicTowers(myMap.mTabBox[index].centerBox);
                }
                break;
            case 1:
                if (player.money >= constPriceSlowingT)
                {
                    tower = new SlowingTowers(myMap.mTabBox[index].centerBox);
                }
                break;
            case 2:
                if (player.money >= constPriceExplosiveT)
                {
                    tower = new ExplosiveTowers(myMap.mTabBox[index].centerBox);
                }
                break;
            case 4:
                if (player.money >= constPriceSniperT)
                {
                    tower = new SniperTowers(myMap.mTabBox[index].centerBox);
                }
            }
            // Condition to prevent segfault if the mouse is moved at the last minute when destroying a tower
            if (sTowerType != 3)
            {
                player.money -= tower->price;
                towersArray.push_back(tower);
                myMap.mTabBox[index].isFree = false;
                myMap.mTabBox[index].hasScenary = false;
            }
        }
        // Destroy a tower + give back half of its price
        if (sTowerType == 3)
        {
            int indexTower;
            int indexTodestroy = -1;
            for (unsigned int i = 0; i < towersArray.size(); i++)
            {
                indexTower = myMap.FindBox(towersArray[i]->position.x, towersArray[i]->position.y);
                if (indexTower == index)
                {
                    indexTodestroy = i;
                }
            }
            for (auto it = towersArray.begin(); it != towersArray.end(); it++)
            {
                if (indexTodestroy != -1 && *it == towersArray[indexTodestroy])
                {
                    myMap.mTabBox[index].isFree = true;
                    player.money += towersArray[indexTodestroy]->price / 2 + ((towersArray[indexTodestroy]->upgradePrice / 4 * (towersArray[indexTodestroy]->level - 1)));
                    Towers *tower = towersArray[indexTodestroy];
                    towersArray.erase(it);
                    delete tower;
                    break;
                }
            }
        }
        // If the mouse button is up, stop the drag and drop
        if (sTowerType >= 0 && IsMouseButtonUp(MOUSE_BUTTON_LEFT))
        {
            sTowerType = -1;
        }
    }
}

void Game ::DrawDragTower(int sTowerType, Resource &resource)
{
    float textureScaleX = resource.tilesheet.width / constTileSheetWidth;
    float textureScaleY = resource.tilesheet.height / constTileSheetHeight;
    int tileX, tileY, tileXDet, tileYDet = 0;
    switch (sTowerType)
    {
    case 0:
        tileX = 20;
        tileY = 10;
        DrawTextureRec(resource.tilesheet, {(float)tileX * constSizeBox, (float)tileY * constSizeBox, textureScaleX, textureScaleY}, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, WHITE);
        DrawCircle(GetMousePosition().x, GetMousePosition().y, constSizeBox * 2.25, {80, 80, 80, 50});
        break;
    case 1:
        tileX = 19;
        tileY = 10;
        DrawTextureRec(resource.tilesheet, {(float)tileX * constSizeBox, (float)tileY * constSizeBox, textureScaleX, textureScaleY}, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, WHITE);
        DrawCircle(GetMousePosition().x, GetMousePosition().y, constSizeBox * 1.75, {80, 80, 80, 50});
        break;
    case 2:
        tileX = 20;
        tileY = 8;
        DrawTextureRec(resource.tilesheet, {(float)tileX * constSizeBox, (float)tileY * constSizeBox, textureScaleX, textureScaleY}, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, WHITE);
        DrawCircle(GetMousePosition().x, GetMousePosition().y, constSizeBox * 1.75, {80, 80, 80, 50});
        break;
    case 3:
        DrawTextureEx(resource.hammer, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, 0, 0.40, WHITE);
        break;
    case 4:
        tileX = 19;
        tileY = 10;
        tileXDet = 20;
        tileYDet = 9;
        DrawTextureRec(resource.tilesheet, {(float)tileX * constSizeBox, (float)tileY * constSizeBox, textureScaleX, textureScaleY}, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, BLACK);
        DrawTextureRec(resource.tilesheet, {(float)tileXDet * constSizeBox, (float)tileYDet * constSizeBox, textureScaleX, textureScaleY}, {GetMousePosition().x - constSizeBox / 2, GetMousePosition().y - constSizeBox / 2}, DARKBLUE);
        DrawCircle(GetMousePosition().x, GetMousePosition().y, constSizeBox * 30, {80, 80, 80, 50});
        break;
    }
}

void Game::UpdateTower(std::vector<Towers *> &towersArray, std::vector<Enemies *> &enemiesArray, Resource &resource, Player &player)
{
    mTimeCounter++;
    for (unsigned int i = 0; i < towersArray.size(); i++)
    {
        towersArray[i]->Draw(resource.tilesheet);
        towersArray[i]->Shoot(enemiesArray, towersArray, resource);
        towersArray[i]->ChooseTarget(enemiesArray);

        if (myMap.FindBox(GetMousePosition().x, GetMousePosition().y) == myMap.FindBox(towersArray[i]->position.x, towersArray[i]->position.y))
        {
            if (player.money >= towersArray[i]->upgradePrice && towersArray[i]->level < 6)
            {
                DrawText(TextFormat("Level : %d\nUpgrade : %d $", towersArray[i]->level, towersArray[i]->upgradePrice), towersArray[i]->position.x - constSizeBox / 2, towersArray[i]->position.y - constSizeBox / 2, 15, GOLD);
            }
            else if (player.money < towersArray[i]->upgradePrice)
            {
                DrawText(TextFormat("Level : %d\nUpgrade : %d $", towersArray[i]->level, towersArray[i]->upgradePrice), towersArray[i]->position.x - constSizeBox / 2, towersArray[i]->position.y - constSizeBox / 2, 15, RED);
            }
            else
            {
                DrawText(TextFormat("Level : Level Max\nUpgrade : Impossible"), towersArray[i]->position.x - constSizeBox / 2, towersArray[i]->position.y - constSizeBox / 2, 15, RED);
            }
            DrawText(TextFormat("To upgrade :\n Right click\n on the tower\n"), constScreenWidth + 30, constScreenHeight / 8 + constSizeBox * 8 + constSizeBox, 18, GOLD);

            DrawCircle(towersArray[i]->position.x, towersArray[i]->position.y, towersArray[i]->range, {80, 80, 80, 50});
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && player.money >= towersArray[i]->upgradePrice && towersArray[i]->level < 6)
            {
                player.money -= towersArray[i]->upgradePrice;
                towersArray[i]->Upgrade();
            }
        }
    }
}

void Game::DrawShop(Resource &resource, Player &player)
{
    // Reused variables
    float textureScaleX = resource.tilesheet.width / constTileSheetWidth;
    float textureScaleY = resource.tilesheet.height / constTileSheetHeight;
    float middleShopBox = constScreenWidth + 70;
    float middleShopText = constScreenWidth + 80;
    float heightScaleShop = constScreenHeight / 10;
    int fontSize = 20;

    // Draw texture of Towers on shop
    DrawTextureRec(resource.backgroundMenu, {50, 0, 200, constScreenHeight}, {constScreenWidth, 0}, RAYWHITE);
    DrawTextureRec(resource.tilesheet, {20 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY}, {middleShopBox, heightScaleShop - constSizeBox}, WHITE);
    DrawTextureRec(resource.tilesheet, {19 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY}, {middleShopBox, heightScaleShop + constSizeBox}, WHITE);
    DrawTextureRec(resource.tilesheet, {20 * constSizeBox, 8 * constSizeBox, textureScaleX, textureScaleY}, {middleShopBox, heightScaleShop + constSizeBox * 5}, WHITE);
    DrawTextureRec(resource.tilesheet, {19 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY}, {middleShopBox, heightScaleShop + constSizeBox * 3}, BLACK);
    DrawTextureRec(resource.tilesheet, {20 * constSizeBox, 9 * constSizeBox, textureScaleX, textureScaleY}, {middleShopBox, heightScaleShop + constSizeBox * 3}, DARKBLUE);
    DrawTextureEx(resource.hammer, {middleShopText, heightScaleShop + constSizeBox * 7}, 0, 0.40, WHITE);
    //Switch text and rectangle from Green to Red depending on player money
    if (player.money < constPriceSniperT)
    {
        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox * 3, constSizeBox, constSizeBox, RED);
        DrawText(TextFormat("%d $", constPriceSniperT), middleShopText, heightScaleShop + constSizeBox * 4, fontSize, RED);
    }
    else
    {
        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox * 3, constSizeBox, constSizeBox, GREEN);
        DrawText(TextFormat("%d $", constPriceSniperT), middleShopText, heightScaleShop + constSizeBox * 4, fontSize, WHITE);
    }
    if (player.money < constPriceExplosiveT)
    {

        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox * 5, constSizeBox, constSizeBox, RED);
        DrawText(TextFormat("%d $", constPriceExplosiveT), middleShopText, heightScaleShop + constSizeBox * 6, fontSize, RED);
    }
    else
    {
        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox * 5, constSizeBox, constSizeBox, GREEN);
        DrawText(TextFormat("%d $", constPriceExplosiveT), middleShopText, heightScaleShop + constSizeBox * 6, fontSize, WHITE);
    }

    if (player.money < constPriceSlowingT)
    {

        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox, constSizeBox, constSizeBox, RED);
        DrawText(TextFormat("%d $", constPriceSlowingT), middleShopText, heightScaleShop + constSizeBox * 2, fontSize, RED);
    }
    else
    {
        DrawRectangleLines(middleShopBox, heightScaleShop + constSizeBox, constSizeBox, constSizeBox, GREEN);
        DrawText(TextFormat("%d $", constPriceSlowingT), middleShopText, heightScaleShop + constSizeBox * 2, fontSize, WHITE);
    }

    if (player.money < constPriceClassicT)
    {
        DrawRectangleLines(middleShopBox, heightScaleShop - constSizeBox, constSizeBox, constSizeBox, RED);
        DrawText(TextFormat("%d $", constPriceClassicT), middleShopText, heightScaleShop, fontSize, RED);
    }
    else
    {
        DrawRectangleLines(middleShopBox, heightScaleShop - constSizeBox, constSizeBox, constSizeBox, GREEN);
        DrawText(TextFormat("%d $", constPriceClassicT), middleShopText, heightScaleShop, fontSize, WHITE);
    }
}

int Game::GenerateExplosion(Resource &resource)
{

    DrawTexturePro(resource.explosion, {32.f * mAnimIndexExplosion, 0, 32, 32}, {/* positionX, positionY */ myMap.roads.back().RoadBox->pBox.x, myMap.roads.back().RoadBox->pBox.y, 64, 64}, {0, 0}, 0, RAYWHITE);

    return mAnimIndexExplosion++;
}

void Game::DrawPlayer(Resource &resource, Player &player)
{
    // Player info
    float playerTextX = constScreenWidth - 325;
    DrawTextureRec(resource.playerUI, {(float)resource.playerUI.width / 2, (float)resource.playerUI.height, (float)resource.playerUI.width / 2, (float)resource.playerUI.height / 2}, {constScreenWidth - 375, -50}, WHITE);
    DrawText(TextFormat("PLAYER            ROUND %d", mCurrentRound), playerTextX, 40, 20, WHITE);
    DrawText(TextFormat("  LIFE  : %d/%d", player.life, player.lifeMax), playerTextX, 70, 20, GREEN);
    DrawText(TextFormat("  MONEY : %d", player.money), playerTextX, 100, 20, GOLD);
    //Healthbar visual
    DrawRectangle(constScreenWidth - 150, 70, 100, 20, BLACK);
    float sizePlayerHealth = (player.life * 100) / player.lifeMax;
    if (sizePlayerHealth <= 100 * 0.25)
    {
        DrawRectangle(constScreenWidth - 150, 70, sizePlayerHealth, 20, RED);
    }
    else if (sizePlayerHealth <= 100 * 0.5)
    {
        DrawRectangle(constScreenWidth - 150, 70, sizePlayerHealth, 20, ORANGE);
    }
    else if (sizePlayerHealth <= 100 * 0.75)
    {
        DrawRectangle(constScreenWidth - 150, 70, sizePlayerHealth, 20, YELLOW);
    }
    else
    {
        DrawRectangle(constScreenWidth - 150, 70, sizePlayerHealth, 20, GREEN);
    }
}

void Game::DrawLineOnMap(Resource &resource)
{
    //Draw info on map to help choose towers's position
    for (float i = 1; i < constBoxRow; i += 1)
    {
        DrawLine(0, i * constSizeBox, constScreenWidth, i * constSizeBox, GREEN);
    }
    for (float i = 1; i < constBoxColumn; i += 1)
    {
        DrawLine(i * constSizeBox, 0, i * constSizeBox, constScreenHeight, GREEN);
    }
    for (size_t i = 0; i < myMap.roads.size(); i++)
    {
        if (myMap.roads[i].RoadBox->tmpRoad)
        {
            DrawTexturePro(resource.tilesheet, {13 * constSizeBox, 12 * constSizeBox, (float)resource.tilesheet.width / constTileSheetWidth, (float)resource.tilesheet.height / constTileSheetHeight}, {myMap.roads[i].RoadBox->centerBox.x, myMap.roads[i].RoadBox->centerBox.y, constSizeBox, constSizeBox}, {constSizeBox / 2, constSizeBox / 2}, 45, {230, 41, 55, 100});
        }
    }
}
