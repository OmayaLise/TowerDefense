#pragma once
#include "../entities.hpp"

enum class classEnemy
{
    E_CLASSIC,
    E_STRONG,
    E_HEAL,
    E_BOSS,
    E_TELEPORT,
    E_NOT_SET,
};


// Virtual class for enemies
class Enemies : public Entities
{
protected:
    int mPreviousLife; 
    size_t mBoxWalkedId;
    vector2D mPreviousCenter;
    float mSpeedLimit;
    float mTimeCounter;
    int mLifeMax;
public:
    int enemyId;
    float timeDeadAnimation;
    int drop;
    //isInit allow us to know if the enemy has been properly spwan or not
    bool isInit = false;
    float speed;

    int rotation;
    int life;
    
    //Constructor and Destructor
    Enemies(vector2D position, float nSpeed, int nDrop, int nLife, int nDamage, int curRound);
    virtual ~Enemies() {};

    //Class function
    virtual void Heal(std::vector<Enemies *>& ally) = 0;
    virtual bool Move(Map& myMap,Resource& resource) = 0;
    virtual void Draw(Resource& resource) = 0;
    virtual int DoDamage() = 0;
    virtual int GetMaxLife() = 0;
    virtual classEnemy GetClass() = 0;
    virtual void ResetSpeed() = 0;

};

//===========================================================
//                      HERITED


// Basic enemy (medium speed, medium attack)
class ClassicEnemies : public Enemies
{
private:
    /* data */
public:
    ClassicEnemies(vector2D spawnPosition, int curRound);
    ~ClassicEnemies() override;

    classEnemy GetClass() override;
    bool Move(Map& myMap,Resource& resource) override;
    void Draw(Resource& resource) override;
    void Heal(std::vector<Enemies *>& ally) override;
    int GetMaxLife() override;
    int DoDamage() override;
    void ResetSpeed() override;
};



// Strong enemy (slow speed, heavy mDamage)
class StrongEnemies : public Enemies
{
private:
    /* data */
public:
    StrongEnemies(vector2D spawnPosition, int curRound);
    ~StrongEnemies() override;

    classEnemy GetClass() override;
    void Heal(std::vector<Enemies *>& ally) override;
    bool Move(Map& myMap,Resource& resource) override;
    void Draw(Resource& resource) override;
    int GetMaxLife() override;
    int DoDamage() override;
    void ResetSpeed() override;
};



// Healing enemy (fast speed, low mDamage + ability to heal around him)
class HealingEnemies : public Enemies
{
private:
    /* data */
    const int mHealValue = 5;
    const int mRange = constSizeBox;
    float mHealCounter;
public:
    HealingEnemies(vector2D spawnPosition, int curRound);
    ~HealingEnemies() override;

    classEnemy GetClass() override;
    void Heal(std::vector<Enemies *>& ally) override;
    bool Move(Map& myMap,Resource& resource) override;
    void Draw(Resource& resource) override;
    int GetMaxLife() override;
    int DoDamage() override;
    void ResetSpeed() override;
};

class BossEnemies : public Enemies
{
private :

public :
    BossEnemies(vector2D spawnPosition, int curRound);
    ~BossEnemies() override;

    classEnemy GetClass() override;
    bool Move(Map& myMap,Resource& resource) override;
    void Draw(Resource& resource) override;
    void Heal(std::vector<Enemies *>& ally) override;
    int GetMaxLife() override;
    int DoDamage() override;
    void ResetSpeed() override;
};

class TeleportEnemy : public Enemies
{
private :
    float mTeleportCounter = 0;
    int mTeleportRange;
public :
    TeleportEnemy(vector2D spawnPosition, int curRound);
    ~TeleportEnemy() override;

    classEnemy GetClass() override;
    bool Move(Map& myMap,Resource& resource) override;
    void Draw(Resource& resource) override;
    void Heal(std::vector<Enemies *>& ally) override;
    int GetMaxLife() override;
    int DoDamage() override;
    void ResetSpeed() override;
};


