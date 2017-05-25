#pragma once
#include "geom.h"

class LightPoint{
public:
    LightPoint(Point position_, long double power_) : position(position_), power(power_) {};
    Point position;
    long double power;
};

