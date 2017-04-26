#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "math.h"
#include "geom.h"
class LightSource {
public:
    double intensity;
    ThreeDVector centr;
    LightSource(double _intensity, ThreeDVector _centr): intensity(_intensity),  centr(_centr){}
    double calcBrightness(ThreeDVector targetPoint, GeomObj* figure) {
        ThreeDVector ray = centr - targetPoint;
        double dist2 = ray.len();
        ThreeDVector norm = figure->normal_in_point(targetPoint);
        double cosNorm = std::fabs(scalar_multiply(ray,norm))/sqrt(ray.len() * norm.len());
        return (intensity * cosNorm)/dist2;
    }
};


#endif // LIGHT_H
