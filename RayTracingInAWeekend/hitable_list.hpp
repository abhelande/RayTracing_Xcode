//
//  hitable_list.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef hitable_list_h
#define hitable_list_h

#include "hitable.hpp"
#include <vector>

class scene: public object  {
public:
    scene() {}
    scene(std::vector<object*> &l) {objects = l; }
    virtual bool hit(const ray& r, float tmin, float tmax, intersectParams& rec) const;
    std::vector<object*> objects;
};

bool scene::hit(const ray& r, float t_min, float t_max, intersectParams& rec) const {
    intersectParams temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif /* hitable_list_h */
