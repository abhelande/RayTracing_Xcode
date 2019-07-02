//
//  ray.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2018 Abhijit Bhelande. All rights reserved.
//

#ifndef ray_hpp
#define ray_hpp

#include "vec3.hpp"

class ray {
public:
    vec3 A; // origin
    vec3 B; // direction
    
    ray() : A(vec3()), B(vec3()) {}
    ray(const vec3 &origin, const vec3 &dir) { A = origin; B = dir; }
    
    inline vec3 origin() const { return A; }
    inline vec3 direction() const { return B; }
    
    vec3 point_at_parameter(float t) const {
        return (A + t * B);
    }
};

#endif /* ray_hpp */
