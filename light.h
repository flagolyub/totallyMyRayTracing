#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "geom.h"
class PointLightSource{
public:
    PointLightSource(Point pos_, long double power_) : pos(pos_), power(power_) {};
    Point pos;
    long double power; //в канделах
};

#endif // LIGHT_H
