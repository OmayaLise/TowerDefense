
#include "towers.hpp"

// Virtual class for Towers
Towers::Towers(vector2D validPosition, int nDamage) : Entities(validPosition, nDamage)
{
}
void Towers::Draw(Texture2D &texture)
{
}

void Towers ::DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray)
{
}

// Check if enemy is in range and alive
void Towers ::ChooseTarget(std::vector<Enemies *> const &enemiesArray)
{
    bool isEnemyTargeted = false;
    for (unsigned int i = 0; i < enemiesArray.size(); i++)
    {
        if ((VectorCompareCircle(position, enemiesArray[i]->position, range)) && enemiesArray[i]->life > 0)
        {
            enemyTarget = enemiesArray[i];
            isEnemyTargeted = true;

            break;
        }
    }
    if (isEnemyTargeted == false)
    {
        enemyTarget = nullptr;
    }
}

// Shoot if enemyTarget exist
void Towers ::Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource)
{
}

// Upgrade tower
void Towers ::Upgrade()
{
    level++;
    mDamage += mDamage / 4;
    upgradePrice += price / 2;
    if (level <= 3)
    {
        range += constSizeBox / 2;
    }
}

void Towers ::DrawBase(Texture2D texture, int &level)
{
    float textureScaleX = texture.width / constTileSheetWidth;
    float textureScaleY = texture.height / constTileSheetHeight;
    Color squareColor;
    float typeXSqua;
    float typeYSqua = 7;

    switch (level)
    {
    case 1:
        squareColor = WHITE;
        typeXSqua = 22;
        break;
    case 2:
        squareColor = LIGHTGRAY;
        typeXSqua = 22;
        break;
    case 3:
        squareColor = DARKGRAY;
        typeXSqua = 22;
        break;
    case 4:
        squareColor = WHITE;
        typeXSqua = 20;
        break;
    case 5:
        squareColor = LIGHTGRAY;
        typeXSqua = 20;
        break;
    case 6:
        squareColor = DARKGRAY;
        typeXSqua = 20;
        break;
    default:
        squareColor = DARKGRAY;
        typeXSqua = 20;
        break;
    }

    Rectangle squareRec = {typeXSqua * constSizeBox, typeYSqua * constSizeBox, textureScaleX, textureScaleY};
    Rectangle destinationRec = {(float)position.x, (float)position.y, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawTexturePro(texture, squareRec, destinationRec, origin, 0, squareColor);
}

// --------------------------------------------------------------------------
ClassicTowers::ClassicTowers() : Towers({-1, -1}, 1)
{
}

ClassicTowers::ClassicTowers(vector2D validPosition) : Towers(validPosition, 20)
{
    mShootSpeed = 0.5;
    mRotation = 0;
    mShootTime = 0;
    mAnimTime = 0;
    level = 1;
    type = T_CLASSIC;
    price = constPriceClassicT;
    upgradePrice = constPriceClassicT / 2;
    range = constSizeBox * 2.25;
    enemyTarget = nullptr;
}

void ClassicTowers::Draw(Texture2D &texture)
{
    float textureScaleX = texture.width / constTileSheetWidth;
    float textureScaleY = texture.height / constTileSheetHeight;

    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle towerRec = {20 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY};
    Rectangle destinationRec = {position.x, position.y, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawBase(texture, level);
    DrawTexturePro(texture, towerRec, destinationRec, origin, mRotation + 90, WHITE);
}
void ClassicTowers ::DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray)
{
    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle shootRec = {19 * (float)constSizeBox, 12 * constSizeBox, (float)texture.width / 23, (float)texture.height / 13};
    Rectangle destinationRec = {(float)position.x + (float)cos(DEG2RAD * mRotation) * constSizeBox / 2, (float)position.y + (float)sin(DEG2RAD * mRotation) * constSizeBox / 2, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawTexturePro(texture, shootRec, destinationRec, origin, mRotation + 90, WHITE);
}
void ClassicTowers::Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource)
{
    mShootTime += GetFrameTime();
    mAnimTime += GetFrameTime();
    if (enemyTarget != nullptr)
    {
        if (mShootTime > mShootSpeed)
        {

            enemyTarget->life -= mDamage;
            resource.music.MyPlaySound(SoundType::ClassicShoot);
            mShootTime = 0;
        }
        if (mAnimTime > mShootSpeed)
        {
            DrawShoot(resource.tilesheet, towersArray);
            if (mAnimTime > mShootSpeed + 0.2)
            {
                mAnimTime = mShootTime;
            }
        }
    }
}

// --------------------------------------------------------------------------
SlowingTowers::SlowingTowers() : Towers({-1, -1}, 1)
{
}

SlowingTowers::SlowingTowers(vector2D validPosition) : Towers(validPosition, 10)
{
    mShootSpeed = 0.25;
    mRotation = 0;
    mShootTime = 0;
    mAnimTime = 0;
    level = 1;
    type = T_SLOWING;
    price = constPriceSlowingT;
    upgradePrice = price / 2;
    range = constSizeBox * 1.75;
    enemyTarget = nullptr;
}
void SlowingTowers::Draw(Texture2D &texture)
{
    float textureScaleX = texture.width / constTileSheetWidth;
    float textureScaleY = texture.height / constTileSheetHeight;

    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle towerRec = {19 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY};
    Rectangle destinationRec = {position.x, position.y, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawBase(texture, level);
    DrawTexturePro(texture, towerRec, destinationRec, origin, mRotation + 90, WHITE);
}
void SlowingTowers ::DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray)
{
    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle shootRec = {21 * (float)constSizeBox, 12 * constSizeBox, (float)texture.width / 23, (float)texture.height / 13};
    Rectangle destinationRec = {(float)position.x + (float)cos(DEG2RAD * mRotation) * constSizeBox / 2, (float)position.y + (float)sin(DEG2RAD * mRotation) * constSizeBox / 2, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawTexturePro(texture, shootRec, destinationRec, origin, mRotation + 90, GREEN);
}
void SlowingTowers::Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource)
{
    mShootTime += GetFrameTime();
    mAnimTime += GetFrameTime();
    if (enemyTarget != nullptr)
    {
        if (mShootTime > mShootSpeed)
        {
            enemyTarget->life -= mDamage;
            mShootTime = 0;
            if (enemyTarget->speed > 2)
            {
                enemyTarget->speed -= enemyTarget->speed / 3;
            }
        }
        if (mAnimTime > mShootSpeed)
        {
            DrawShoot(resource.tilesheet, towersArray);
            resource.music.MyPlaySound(SoundType::SlowShoot);
            if (mAnimTime > mShootSpeed + 0.2)
            {
                mAnimTime = mShootTime;
            }
        }
    }
}

// --------------------------------------------------------------------------
ExplosiveTowers::ExplosiveTowers() : Towers({-1, -1}, 1)
{
}

ExplosiveTowers::ExplosiveTowers(vector2D validPosition) : Towers(validPosition, 40)
{
    mShootSpeed = 0.75;
    mRotation = 0;
    mShootTime = 0;
    mAnimTime = 0;
    level = 1;
    type = T_EXPLOSIVE;
    price = constPriceExplosiveT;
    upgradePrice = price / 2;
    range = constSizeBox * 1.75;
    enemyTarget = nullptr;
}

void ExplosiveTowers::Draw(Texture2D &texture)
{
    float textureScaleX = texture.width / constTileSheetWidth;
    float textureScaleY = texture.height / constTileSheetHeight;

    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle towerRec = {20 * constSizeBox, 8 * constSizeBox, textureScaleX, textureScaleY};
    Rectangle destinationRec = {position.x, position.y, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawBase(texture, level);
    DrawTexturePro(texture, towerRec, destinationRec, origin, mRotation + 90, WHITE);
}
void ExplosiveTowers ::DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray)
{
    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle shootRec = {20 * (float)constSizeBox, 12 * constSizeBox, (float)texture.width / 23, (float)texture.height / 13};
    Rectangle destinationRec = {(float)position.x + (float)cos(DEG2RAD * mRotation) * constSizeBox / 2, (float)position.y + (float)sin(DEG2RAD * mRotation) * constSizeBox / 2, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawTexturePro(texture, shootRec, destinationRec, origin, mRotation + 90, WHITE);
}
void ExplosiveTowers::Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource)
{
    mShootTime += GetFrameTime();
    mAnimTime += GetFrameTime();

    if (enemyTarget != nullptr)
    {
        if (mShootTime > mShootSpeed)
        {
            enemyTarget->life -= mDamage;
            mShootTime = 0;
            for (unsigned int i = 0; i < enemiesArray.size(); i++)
            {
                if ((VectorCompareCircle(enemyTarget->position, enemiesArray[i]->position, constSizeBox * 1.25)) && enemiesArray[i]->isInit)
                {
                    if(enemiesArray[i] != enemyTarget)
                    {
                        enemiesArray[i]->life -= mDamage;
                        DrawCircle(enemyTarget->position.x, enemyTarget->position.y, constSizeBox * 1.25, {230, 41, 55, 230});
                    }
                }
            }
        }
        if (mAnimTime > mShootSpeed)
        {
            DrawShoot(resource.tilesheet, towersArray);
            resource.music.MyPlaySound(SoundType::ExplosiveShoot);
            if (mAnimTime > mShootSpeed + 0.3)
            {
                mAnimTime = mShootTime;
            }
        }
    }
}

// --------------------------------------------------------------------------
SniperTowers::SniperTowers() : Towers({-1, -1}, 1)
{
}

SniperTowers::SniperTowers(vector2D validPosition) : Towers(validPosition, 50)
{
    mShootSpeed = 1.5;
    mRotation = 0;
    mShootTime = 0;
    mAnimTime = 0;
    level = 1;
    type = T_SNIPER;
    price = constPriceSniperT;
    upgradePrice = price / 2;
    range = constSizeBox * 30;
    enemyTarget = nullptr;
}

void SniperTowers::Draw(Texture2D &texture)
{
    float textureScaleX = texture.width / constTileSheetWidth;
    float textureScaleY = texture.height / constTileSheetHeight;

    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }

    Rectangle detRec = {20 * constSizeBox, 9 * constSizeBox, textureScaleX, textureScaleY};
    Rectangle towerRec = {19 * constSizeBox, 10 * constSizeBox, textureScaleX, textureScaleY};
    Rectangle destinationRec = {position.x, position.y, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};

    DrawBase(texture, level);
    DrawTexturePro(texture, towerRec, destinationRec, origin, mRotation + 90, BLACK);
    DrawTexturePro(texture, detRec, destinationRec, origin, mRotation + 90, DARKBLUE);
}
void SniperTowers ::DrawShoot(Texture2D &texture, std::vector<Towers *> const &towersArray)
{
    if (enemyTarget != nullptr)
    {
        mRotation = atan2f(enemyTarget->position.y - position.y, enemyTarget->position.x - position.x) * radToDeg;
    }
    Rectangle shootRec = {21 * (float)constSizeBox, 12 * constSizeBox, (float)texture.width / 23, (float)texture.height / 13};
    Rectangle destinationRec = {(float)position.x + (float)cos(DEG2RAD * mRotation) * constSizeBox / 2, (float)position.y + (float)sin(DEG2RAD * mRotation) * constSizeBox / 2, constSizeBox, constSizeBox};
    Vector2 origin = {constSizeBox / 2, constSizeBox / 2};
    DrawTexturePro(texture, shootRec, destinationRec, origin, mRotation + 90, WHITE);
}
void SniperTowers::Shoot(std::vector<Enemies *> &enemiesArray, std::vector<Towers *> &towersArray, Resource &resource)
{
    mShootTime += GetFrameTime();
    mAnimTime += GetFrameTime();
    if (enemyTarget != nullptr)
    {
        if (mShootTime > mShootSpeed)
        {
            enemyTarget->life -= mDamage;
            resource.music.MyPlaySound(SoundType::SniperShoot);
            mShootTime = 0;
        }
        if (mAnimTime > mShootSpeed)
        {
            DrawShoot(resource.tilesheet, towersArray);
            if (mAnimTime > mShootSpeed + 0.2)
            {
                mAnimTime = mShootTime;
            }
        }
    }
}
