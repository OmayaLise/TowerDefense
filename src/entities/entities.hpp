#pragma once

#include "../../include/math_towerdefense.hpp"
#include "../setup.hpp"
#include "../map.hpp"


class Entities
{
protected:
    float mDamage;
public:
    vector2D position;
    Entities(vector2D nPosition, int nDamage);
    virtual ~Entities();
    
   //virtual int DoDamage();
};

