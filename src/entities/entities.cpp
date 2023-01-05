#include "entities.hpp"

Entities::Entities(vector2D nPosition, int nDamage)
{
    position = nPosition;
    mDamage = nDamage;
}

Entities::~Entities()
{
}
