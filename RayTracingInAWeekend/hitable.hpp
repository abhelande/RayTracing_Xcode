//
//  hitable.h
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef hitable_h
#define hitable_h

#include "ray.hpp"

class material;

struct intersectParams
{
public:
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal;
    material *surfaceMat;
};

class hitable
{
public:
    virtual bool hit(const ray& r,
                     float t_min,
                     float t_max,
                     intersectParams& rec) const = 0;
};


#endif /* hitable_h */
