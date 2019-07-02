//
//  util.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 7/1/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef util_h
#define util_h

#include "vec3.hpp"

#define PI 3.14159265

float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

// This is the same as glsl reflect
vec3 reflect(const vec3& I, const vec3& N)
{
    return I - 2.0f * dot(I, N) * N;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

vec3 unitSphereRandomRadVec()
{
    vec3 rdm;
    do {
        rdm = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1.0f);
    } while (rdm.squared_length() >= 1.0f);
    
    return rdm;
}


#endif /* util_h */
