//
//  util.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef util_h
#define util_h

#include "vec3.hpp"

constexpr float kEpsilon = 1e-8;

inline float degToRad(float degrees)
{
    return degrees * (M_PI / 180.0f);
}

// polynomial approximation by Christophe Schlick
// to approximate reflectivity factor depending on transparent surface
// viewing angle (i.e. glass reflects some light depending on viewing angle)
//
// specular reflection coefficient R can be approximated by:
// R(θ) = R0 + (1 - R0) * ((1 - cos(θ))^5)
// R0 = [(n_in - n_out) / (n_in + n_out)]^2
// below we take n_in = 1
// θ: angle between incident light and the normal at point of incidence
//    i.e. cos(θ) = I.N
float schlick(float cosine,
              float outRefIdx) {
    float r0 = (1.0f - outRefIdx) / (1.0f + outRefIdx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}

// This is the same as glsl reflect
// Reflected = I - 2 * dot (I, N) * N
// I: Incident Ray
// N: Normal at point of incidence
vec3 reflect(const vec3& I, const vec3& N)
{
    return I - 2.0f * dot(I, N) * N;
}

// same as glsl refract
// but needs refracted ray as returnable parameter
// I: incident vector.
// N: Specifies the normal vector.
// eta: ratio of indices of refraction.
// refraction vector, R:
// k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
// if (k < 0.0)
//     R = vec(0.0);
// else
//
//     R = eta * I - (eta * dot(N, I) + sqrt(k)) * N;
//
bool refract(const vec3& I, const vec3& N, float eta, vec3& refracted) {
    vec3 uv = unit_vector(I);
    float dt = dot(uv, N);
    float discriminant = 1.0f - eta * eta * (1.0f - dt * dt);
    if (discriminant > kEpsilon) {
        refracted = eta * (uv - N * dt) - N * sqrt(discriminant);
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
