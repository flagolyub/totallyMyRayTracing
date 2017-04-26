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
        //return (intensity * cosNorm)/dist2;
        return std::max((double)0,(intensity * cosNorm)/dist2);
    }

typedef std::tuple<bool,ThreeDVector,GeomObj*> IntersectionData;
/*double findLitPoint(IntersectionData targetPointData, std::shared_ptr<Kdtree> kdtree) {
    ThreeDVector targetPoint = ThreeDVector(targetPointData);
    ThreeDVector ray = (targetPoint - centr);

    if(ray == ThreeDVector(0,0,0)) {
        return 0;
    }

    IntersectionData realIntersectionData = kdtree->find(ray,centr);

    ThreeDVector firstIntersection = point(realIntersectionData);

    ThreeDVector diff = firstIntersection - targetPoint;

    if(diff.len() > EPS * EPS || status(realIntersectionData) != status(targetPointData) || figure(targetPointData) != figure(realIntersectionData)) {
        return 0;
    }
     return calcBrightness(targetPoint, figure(targetPointData));
 }*/

};

#endif // LIGHT_H
