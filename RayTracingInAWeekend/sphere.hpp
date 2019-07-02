//
//  sphere.h
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include "hitable.hpp"
#include "material.hpp"
#include <cmath>

class sphere : public hitable
{
public:
    sphere() = delete;
    sphere(vec3 cen,
           float rad,
           material* mat) : center(cen),
                            radius(rad),
                            surfaceMat(mat) {}
    
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
    {
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;

        float discriminant = b * b - a * c;
        if (discriminant > 0) {
            bool didHit = false;
            
            // check root 1
            float root = (-b - sqrt(discriminant)) / a;
            if (root < t_max && root > t_min) {
                didHit = true;
            } else {
                // root 2
                root = (-b + sqrt(discriminant)) / a;
                didHit = (root < t_max && root > t_min);
            }
            
            if (didHit) {
                rec.t = root;
                rec.p = r.point_at_parameter(root);
                rec.normal = (rec.p - center) / radius;
                rec.surfaceMat = surfaceMat;
                
                // uv calc (cylindrical coords)
                // divide by (2 x PI) to convert the returned angle to [-0.5, 0.5] range
                // N.y = v
                // 0.5 add to shift to [0,1] range
                rec.u = atan2(rec.normal.x(), rec.normal.z()) / (2 * M_PI) + 0.5f;
                rec.v = rec.normal.y() * 0.5f + 0.5f;
                return true;
            }
        }
        
        return false;
    }
    
    vec3 center;
    float radius;
    material *surfaceMat;
};


#endif /* sphere_h */
