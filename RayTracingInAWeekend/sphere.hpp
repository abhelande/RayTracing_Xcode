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
        // For sphere whose center is located at C (<Cx, Cy, Cz>) with Radius R (scalar),
        // Eqn for point P (<Px, Py, Pz>) on the spheres surface,
        // (Px - Cx)^2 + (Py - Cy)^2 + (Px - Cx)^2 = R^2
        // i.e. (P - C).(P - C) - R^2 = 0
        //
        // Substitute P = Ray eqn = O + t*D
        // you get quadratic form: a*x^2 + b*x + c = 0
        // a = |D|^2
        // b = (O - C).(D)
        // c = |(O - C)|^2 - R^2
        //
        // solving this,
        // discriminant < 0 => no intersection
        // otherwise use roots to determine point of intersection
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;
        
        if (discriminant > 0) {
            bool didHit = false;
            float root = 0.0f;
            
            // 0 discriminant
            if (discriminant < kEpsilon) {
                root = -b / a * 0.5f;
                didHit = true;
            } else {
                // check root 1
                root = (-b - sqrt(discriminant)) / a;
                if (root < t_max && root > t_min) {
                    didHit = true;
                } else {
                    // root 2
                    root = (-b + sqrt(discriminant)) / a;
                    didHit = (root < t_max && root > t_min);
                }
            }
            
            if (didHit) {
                rec.t = root;
                // root is used to find point of intersection
                rec.p = r.point_at_parameter(root);
                // normal is simply outwards from center to that point
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
