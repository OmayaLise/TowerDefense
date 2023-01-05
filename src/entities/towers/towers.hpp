#pragma once

#include "../enemies/enemies.hpp"

enum TowerType
{
    T_CLASSIC,
    T_SLOWING,
    T_EXPLOSIVE,
    T_SNIPER
};

// Virtual class for towers
class Towers : public Entities
{
protected:
    float mShootSpeed;
    float mRotation;
    float mShootTime;
    float mAnimTime;

public:
    int level;
    TowerType type;
    int price;
    int upgradePrice;
    float range;
    Enemies *enemyTarget;
    Towers(vector2D validPosition, int nDamage);
    virtual void Draw(Texture2D &texture);
    virtual void DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray);
    virtual void ChooseTarget(std::vector<Enemies *> const &enemiesArray);
    virtual void Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource);
    void Upgrade();
    void DrawBase(Texture2D texture, int &level);
};

// --------------------------------------------------------------------------
// Basic tower (medium speed, medium mDamage, no effect, medium cost)
class ClassicTowers : public Towers
{
public:
    ClassicTowers();
    ClassicTowers(vector2D validPosition);
    void Draw(Texture2D &texture) override;
    void DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray) override;
    void Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource) override;
};

// --------------------------------------------------------------------------
// Slowing tower (fast speed, low mDamage, ability to slow enemies, medium+ cost)
class SlowingTowers : public Towers
{
public:
    SlowingTowers();
    SlowingTowers(vector2D validPosition);
    void Draw(Texture2D &texture) override;
    void DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray) override;
    void Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource) override;
};

// --------------------------------------------------------------------------
// Explosive tower (slow speed, heavy mDamage, heavy cost + ability to attack in zone, high cost)
class ExplosiveTowers : public Towers
{
public:
    ExplosiveTowers();
    ExplosiveTowers(vector2D validPosition);
    void Draw(Texture2D &texture) override;
    void DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray) override;
    void Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource) override;
};

// --------------------------------------------------------------------------
// Sniper tower (slow speed, low mDamage, huge range, medium+ cost)
class SniperTowers : public Towers
{
public:
    SniperTowers();
    SniperTowers(vector2D validPosition);
    void Draw(Texture2D &texture) override;
    void DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray) override;
    void Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource) override;
};
