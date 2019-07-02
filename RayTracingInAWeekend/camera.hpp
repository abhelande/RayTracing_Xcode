//
//  camera.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/19/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef camera_h
#define camera_h

class camera
{
public:
    camera() :
        lowerLeft(vec3(-2.0f, -1.0f, -1.0f)),
        horizontal(vec3(4.0f, 0.0f, 0.0f)),
        vertical(vec3(0.0f, 2.0f, 0.0f)),
        origin(vec3(0.0f, 0.0f, 0.0f))
    {}
    
    ray getRayAt(float u, float v) {
        return ray(origin, (lowerLeft - origin) + u * horizontal + v * vertical);
    }
    
    const vec3 lowerLeft;
    const vec3 horizontal;
    const vec3 vertical;
    const vec3 origin;
};


#endif /* camera_h */
