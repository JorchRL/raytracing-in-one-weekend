//
// Created by Jorge on 12/16/2024.
//

#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class ray {
private:
    point3 orig;
    vec3 dir;
public:
    ray() {};
    ray(const point3& origin, const vec3& direction):
    orig(origin), dir(direction){}

    // const return type means the return value is read only..
    // const keyword after method signature indicates that this
    // method does not mutate any memeber variables (fields)..
    const point3& origin () const  {return orig;}
    const vec3& direction () const  {return dir;}

    // get the point along the ray at distance t from the origin of the ray
    point3 at(double t) const {
        return orig+ t*dir;
    }
};


#endif //RAY_H
