#include "enemies.hpp"

//Set global value to get right number of enemies
static int nbEnemy = 0;

//============================================================================================================================
//                                          Function for enemies

//Get good speed for enemy and increment with round
float MakeSpeedEnemy(float speed ,  int round)
{
    if(round <= 5)
    {
        return speed;
    }
    else
    {
        //Increment by 0.05 each round after round 5
        int multiplicator = 1 + (0.05 * round) ;
        if((speed * multiplicator) <= 20 )
            return speed * multiplicator;
        else 
            return 20; 
    }
}

//Get good speed for enemy and increment with round
int MakeDropEnemy(int drop , int round)
{
    if(round <= 5)
    {
        return drop;
    }
    else
    {
        //Increment by 10% each round after round 5
        int multiplicator = 1  + ((drop/10) * round) ;
        if((drop * multiplicator) <= drop * 3 )
            return drop * multiplicator;
        else 
            return drop*3; 
    }
}

//Setup the enemy pos and update the path already taken
void SetupCurrentPosEnemy(Map& myMap, vector2D& position , vector2D& previousCenter , size_t& boxWalkedId ,int incrementSize)
{    
    //Get the center of the current box of the enemy
    vector2D currentCenter = myMap.mTabBox[myMap.FindBox(position.x , position.y )].centerBox;
    // Set the current Center position
    if(previousCenter.x == -1 && previousCenter.y == -1)
        previousCenter = currentCenter;

    // if enemy walked trough a box increment boxWalked
    if (previousCenter != currentCenter) 
    {
        vector2D vecTest {position, currentCenter};
        //Verification to make sure we walked enough on the box
        if(vecTest.length() < constSizeBox/6 )
        {
            boxWalkedId += incrementSize;
            previousCenter = currentCenter;
        }
    }
}

//Make enemy move in the right direction 
bool MakeMovement(Map& myMap, Enemies& enemy , size_t& boxWalkedId)
{
    //If nextBox exist we move in this direction
    if((myMap.roads.size() - 2) != boxWalkedId )
    {   
        //Calcul the direction with vector () nextBox - vector ( position )
        vector2D direction = {enemy.position , myMap.roads[boxWalkedId+1].RoadBox->centerBox};

        //Normalize then scale and add the direction to move in the right direction
        direction.normalize();
        direction.scale(enemy.speed);
        enemy.position += direction;
        //Compare previous and new direction to know if the direction has changed
        enemy.rotation = atan2f(direction.y , direction.x) * radToDeg;
    }
    else 
    {
        //If the nextbox is null we want to go at the start of the next box to destroy the enemy so we check
        if(myMap.FindBox(enemy.position.x , enemy.position.y ) != myMap.FindBox(myMap.roads[boxWalkedId].RoadBox->centerBox.x , myMap.roads[boxWalkedId].RoadBox->centerBox.y ))
        {
            //If we are in the nextbox we need to destroy the enemy and deal mDamage to the player
            return true;
        }
        else
        {
            //Else we just made it walk trough the last box
            //Calcul the direction with vector {cur->RoadBox->pBox.x + constSizeBox , cur->RoadBox->pBox.y + constSizeBox/2 } - vector ( position )
            vector2D direction = {enemy.position , {myMap.roads.back().RoadBox->centerBox.x, myMap.roads.back().RoadBox->centerBox.y}};
            //Normalize then scale and add the direction to move in the right direction
            direction.normalize();
            direction.scale(enemy.speed);
            enemy.position += direction;
        }
    }
    return false;
}

//Return true if the animation is over and to launch healing process
bool HealAnimation(vector2D position , float& counterHealing, int range)
{
    //Increase Counter
    counterHealing += GetFrameTime();

    //Check counter to not heal at each frame
    if(counterHealing >= 0.5)
    {    
        //Reset color green with lower opacity
        DrawCircle(position.x , position.y , range * 0.5 ,/* Set the color green with less opacity */ {0,228,48 , 128});
        if(counterHealing > 0.55)
        {
            DrawCircle(position.x , position.y , range * 0.75,/* Set the color green with less opacity */ {0,228,48 , 128});
            if(counterHealing > 0.6)
            {

                //Reset counter
                counterHealing = 0;
                DrawCircle(position.x , position.y , range , /* Set the color green with less opacity */{0,228,48 , 128});
                
                return true;
            }
        }
    }
    return false;
}

//Draw Health Bar
void DrawHealthBar(const Enemies& enemy , int lifeMax)
{
    //draw Health bar
    DrawRectangle(enemy.position.x - 20, enemy.position.y - 20 , 40 , 5 , BLACK);
    //Get percentage of health bar
    float sizeHealthBar = (enemy.life * 38) / lifeMax ;
    //If under 25% make bar red
    if(sizeHealthBar <= 38 * 0.25)
    {
        DrawRectangle(enemy.position.x - 19 , enemy.position.y - 19 , sizeHealthBar  , 3 , RED);
    }
    //Under 50% make bar yellow
    else if(sizeHealthBar <= 38 * 0.5)
    {
        DrawRectangle(enemy.position.x - 19 , enemy.position.y - 19 , sizeHealthBar  , 3 , YELLOW);
    }
    //Else make bar green
    else 
    {
        DrawRectangle(enemy.position.x - 19 , enemy.position.y - 19 , sizeHealthBar  , 3 , GREEN);
    }
}

//Draw enemies, healthbar and animation rotating and damage
void DoDraw(Rectangle tileRec, Resource& resource, int& previousLife, int lifeMax, const Enemies& enemy)
{
    //Draw the enemy tinted in red if he took damage
    if(enemy.life < previousLife)
    {
        previousLife = enemy.life;
        DrawTexturePro(resource.tilesheet, tileRec, {enemy.position.x, enemy.position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , enemy.rotation , RED);
    }
    else 
    {
        DrawTexturePro(resource.tilesheet, tileRec, {enemy.position.x, enemy.position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , enemy.rotation , WHITE);
    }

    DrawHealthBar(enemy, lifeMax);

}


//============================================================================================================================
//============================================================================================================================
// Enemy base constructor 
Enemies::Enemies(vector2D position, float nSpeed, int nDrop, int nLife, int nDamage, int curRound) : Entities(position , nDamage)
{
    //Each enemy has it's own
    speed = MakeSpeedEnemy(nSpeed , curRound);
    drop = MakeDropEnemy(nDrop , curRound) ;
    life = nLife + ((nLife/10) * curRound);
    mSpeedLimit = speed;
    mPreviousLife = life;
    mLifeMax = life;

    //Same for all enemy
    mBoxWalkedId = 0;
    mPreviousCenter = {-1 , -1};
    mTimeCounter = 0;
    timeDeadAnimation = 0;
    rotation = 180;
    enemyId = nbEnemy++;

}

//============================================================================================================================
//============================================================================================================================
// Basic enemy (medium speed, medium attack)
ClassicEnemies::ClassicEnemies(vector2D spawnPosition, int curRound) : Enemies(spawnPosition, /* Speed */5 , /* Drop */20 , /* Life */100 , /* Damage */ 10 , /* Round */ curRound )
{

}

ClassicEnemies::~ClassicEnemies()
{
}

bool ClassicEnemies::Move(Map& myMap, Resource& resource)
{
    SetupCurrentPosEnemy(myMap , position , mPreviousCenter , mBoxWalkedId , /* walk range */ 1);

    if(MakeMovement(myMap, *this ,mBoxWalkedId))
        return true;

    ResetSpeed();
    return false;
}

void ClassicEnemies::Draw(Resource& resource) 
{
    //Get the rigth sprite form full image
    Rectangle frameTile = {15 * 64, 640, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};

    DoDraw(frameTile , resource , mPreviousLife , mLifeMax , *this);
}

//get mDamage because private
int ClassicEnemies::DoDamage()
{
    return mDamage;
}

//get class for enum check
classEnemy ClassicEnemies::GetClass()
{
    return classEnemy::E_CLASSIC;
}

//get max life because private
int ClassicEnemies::GetMaxLife()
{
    return mLifeMax;
}

//Needed to make healer work
void ClassicEnemies::Heal(std::vector<Enemies *>& ally){};

//Make speed increase after slow
void ClassicEnemies::ResetSpeed()
{
    mTimeCounter += GetFrameTime();

    if(mTimeCounter >= 0.2 && speed < mSpeedLimit)
    {    
        speed += 0.1;
        mTimeCounter = 0;
    }
}


//============================================================================================================================
//============================================================================================================================
// Strong enemy (slow speed, heavy mDamage)
StrongEnemies::StrongEnemies(vector2D spawnPosition, int curRound)  : Enemies(spawnPosition,  /* Speed */4.5 , /* Drop */30 , /* Life */150 , /* Damage */ 15 , /* Round */ curRound )
{
}

StrongEnemies::~StrongEnemies()
{
}

bool StrongEnemies::Move(Map& myMap, Resource& resource)
{
    SetupCurrentPosEnemy(myMap , position , mPreviousCenter , mBoxWalkedId ,/* walk range */ 1);

    if(MakeMovement(myMap, *this ,mBoxWalkedId))
        return true;

    ResetSpeed();
    return false;
}

void StrongEnemies::Draw(Resource& resource)
{ 
    //Get the rigth sprite form full image
    Rectangle frameTile = {17 * 64, 640, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};
    
    DoDraw(frameTile ,resource , mPreviousLife , mLifeMax , *this);
}

//get mDamage because private
int StrongEnemies::DoDamage()
{
    return mDamage;
}

//Get class for enum check
classEnemy StrongEnemies::GetClass()
{
    return classEnemy::E_STRONG;
}

//Get max life because private
int StrongEnemies::GetMaxLife()
{
    return mLifeMax;
}

//Needed to make healer work
void StrongEnemies::Heal(std::vector<Enemies *>& ally)
{    
};

//Make speed increase after slow
void StrongEnemies::ResetSpeed()
{
    mTimeCounter += GetFrameTime();

    if(mTimeCounter >= 0.2 && speed < mSpeedLimit)
    {    
        speed += 0.1;
        mTimeCounter = 0;
    }
}

//============================================================================================================================
//============================================================================================================================
// Healing enemy (medium speed, low mDamage + ability to heal around him)
HealingEnemies::HealingEnemies(vector2D spawnPosition, int curRound) : Enemies(spawnPosition, /* Speed */5.5 , /* Drop */25 , /* Life */75 , /* Damage */ 5 , /* Round */ curRound)
{
    mHealCounter = 0;
}

HealingEnemies::~HealingEnemies()
{
}

bool HealingEnemies::Move(Map& myMap, Resource& resource)
{
    SetupCurrentPosEnemy(myMap , position , mPreviousCenter , mBoxWalkedId , /* walk range */ 1);

    if(MakeMovement(myMap, *this ,mBoxWalkedId))
        return true;

    ResetSpeed();
    return false;
}

void HealingEnemies::Draw(Resource& resource)
{
    //Get the right sprite on the full image
    Rectangle frameTile = {16 * 64, 640, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};
    
    DoDraw(frameTile , resource , mPreviousLife , mLifeMax , *this);

}

//get mDamage because private
int HealingEnemies::DoDamage()
{
    return mDamage;
}

//Get class for enum check
classEnemy HealingEnemies::GetClass()
{
    return classEnemy::E_HEAL;
}

//Get max life because private
int HealingEnemies::GetMaxLife()
{
    return mLifeMax;
}

//Function heal enemy
void HealingEnemies::Heal(std::vector<Enemies *>& ally)
{

    if(HealAnimation(position , mHealCounter , mRange))
    {
        bool allyFound = false;
        //Loop on all enemy
        for(size_t i = 0 ; i < ally.size() ; i++)
        {
            //Check if enemy checked is this
            if(ally[i]->enemyId == enemyId)
            {
                break;
            }
            //Check if enemy is in the mRange of healer
            if(VectorCompareCircle(position , ally[i]->position, mRange))
            {
                if(ally[i]->life != ally[i]->GetMaxLife())
                {
                    //heal amount of heal value
                    ally[i]->life += mHealValue;
                    //Security to not heal above max life
                    if(ally[i]->life > ally[i]->GetMaxLife())
                    {
                        ally[i]->life = ally[i]->GetMaxLife();
                    }
                    //Set at true to not heal self
                    allyFound = true;
                    
                }
            }
        }
        //If no ally has been found in the mRange heal self
        if(!allyFound)
        {
            //Same method
            life += mHealValue;
            if(life > mLifeMax)
            {
                life = mLifeMax;
            }
           
        
        }
    }
    
}

//Make speed increase after slow
void HealingEnemies::ResetSpeed()
{
    mTimeCounter += GetFrameTime();
    if(mTimeCounter >= 0.2 && speed < mSpeedLimit)
    {    
        speed += 0.1;
        mTimeCounter = 0;
    }
}

//============================================================================================================================
//============================================================================================================================
// Boss enemy (medium speed, medium attack)
BossEnemies::BossEnemies(vector2D spawnPosition, int curRound) : Enemies(spawnPosition, /* Speed */3 , /* Drop */350 , /* Life */3500 , /* Damage */ 35 , /* Round */ curRound )
{
}

BossEnemies::~BossEnemies()
{
}

bool BossEnemies::Move(Map& myMap, Resource& resource)
{
    SetupCurrentPosEnemy(myMap , position , mPreviousCenter , mBoxWalkedId , /* walk range */ 1);

    if(MakeMovement(myMap, *this ,mBoxWalkedId))
        return true;

    ResetSpeed();
    return false;
}

void BossEnemies::Draw(Resource& resource) 
{
    //Get the right sprite on the full image
    Rectangle frameRecBase = {16 * 64, 704, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};
    Rectangle frameRecCanon = {16 * 64, 768, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};

    //Draw the canon hover the base of the tank
    //Draw tinted in red if he loosed life
    if(life < mPreviousLife)
    {
        mPreviousLife = life;
        DrawTexturePro(resource.tilesheet, frameRecBase, {position.x, position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , rotation , RED);
        DrawTexturePro(resource.tilesheet, frameRecCanon, {position.x, position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , rotation , RED);
    }
    else 
    {
        DrawTexturePro(resource.tilesheet, frameRecBase, {position.x, position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , rotation , WHITE);
        DrawTexturePro(resource.tilesheet, frameRecCanon, {position.x, position.y  , constSizeBox , constSizeBox} ,{constSizeBox/2 ,constSizeBox/2} , rotation , WHITE);
    }

    DrawHealthBar(*this, mLifeMax);

}

//get mDamage because private
int BossEnemies::DoDamage()
{
    return mDamage;
}

//get class for enum check
classEnemy BossEnemies::GetClass()
{
    return classEnemy::E_BOSS;
}

//get max life because private
int BossEnemies::GetMaxLife()
{
    return mLifeMax;
}

//Needed to make healer work
void BossEnemies::Heal(std::vector<Enemies *>& ally){};

//Make speed increase after slow
void BossEnemies::ResetSpeed()
{
    mTimeCounter += GetFrameTime();

    if(mTimeCounter >= 0.2 && speed < mSpeedLimit)
    {    
        speed += 0.1;
        mTimeCounter = 0;
    }
}

//============================================================================================================================
//============================================================================================================================
// Teleport enemy (medium speed, medium attack)
TeleportEnemy::TeleportEnemy(vector2D spawnPosition, int curRound) : Enemies(spawnPosition, /* Speed */3 , /* Drop */40 , /* Life */40 , /* Damage */ 10 , /* Round */ curRound )
{
    mTeleportRange = 5;
}

TeleportEnemy::~TeleportEnemy()
{
}

bool TeleportEnemy::Move(Map& myMap, Resource& resource)
{
    rotation = 90; // Always turned to the bottom

    SetupCurrentPosEnemy(myMap, position, mPreviousCenter , mBoxWalkedId , /* walk range */ mTeleportRange);

    //teleport to new tile
    mTeleportCounter += GetFrameTime();
    double teleportSpeed = my_remap(speed, 1 , 20, 0.4 , 0.01);
    if(mTeleportCounter >= teleportSpeed)
    {
        DrawTexturePro(resource.tpParticle , {0,64 , 64 , 64}, {position.x-32 , position.y-32, 64, 64}, {0,0}, 0 , WHITE);

            if(mTeleportCounter >= teleportSpeed + 0.2)
            {
                resource.music.MyPlaySound(SoundType::TeleportSound);
                mTeleportCounter = 0;
                //if next tile is availible else go to castle
                if((myMap.roads.size() - mTeleportRange + 1) != mBoxWalkedId )
                {
                    position = {myMap.roads[mBoxWalkedId + mTeleportRange].RoadBox->centerBox.x , myMap.roads[mBoxWalkedId + mTeleportRange].RoadBox->centerBox.y};
                }
                else
                {
                    position = myMap.roads.back().RoadBox->centerBox;
                    return true;
                }
            
        }
    } 
    
    ResetSpeed();
    return false;
}

void TeleportEnemy::Draw(Resource& resource) 
{
    //Get the right sprite on the full image
    Rectangle frameTile = {18 * 64, 640, (float)resource.tilesheet.width / 23, (float)resource.tilesheet.height / 13};
    
    DoDraw(frameTile , resource , mPreviousLife , mLifeMax , *this);
}

//get mDamage because private
int TeleportEnemy::DoDamage()
{
    return mDamage;
}

//get class for enum check
classEnemy TeleportEnemy::GetClass()
{
    return classEnemy::E_TELEPORT;
}

//get max life because private
int TeleportEnemy::GetMaxLife()
{
    return mLifeMax;
}

//Needed to make healer work
void TeleportEnemy::Heal(std::vector<Enemies *>& ally){};

//Make speed increase after slow
void TeleportEnemy::ResetSpeed()
{
    mTimeCounter += GetFrameTime();

    if(mTimeCounter >= 0.2 && speed < mSpeedLimit)
    {    
        speed += 0.1;
        mTimeCounter = 0;
    }
}