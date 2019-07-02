//
//  material.hpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2019 Abhijit Bhelande. All rights reserved.
//

#ifndef material_h
#define material_h

struct hit_record;

#include "ray.hpp"
#include "hitable.hpp"
#include "texture.hpp"
#include "util.hpp"

class material
{
public:
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

// Lambertian is basic diffuse scattering
// scatter incoming ray in a random direction
// each bounce adds an attenuation
class lambertian : public material
{
public:
    lambertian(const vec3& a) : albedo(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 bounce = rec.p + rec.normal + unitSphereRandomRadVec();
        scattered = ray(rec.p, bounce - rec.p);
        attenuation = albedo;
        return true;
    }
    
    vec3 albedo;
};

// Same as Lambertian
// excpet diffuse albedo is via texture look up
class lambertianTexture : public material
{
public:
    lambertianTexture(texture* a) : albedo(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 bounce = rec.p + rec.normal + unitSphereRandomRadVec();
        scattered = ray(rec.p, bounce - rec.p);
        attenuation = albedo->texelAt(rec.u, rec.v, rec.p);
        return true;
    }
    
    texture* albedo;
};

class metal : public material
{
public:
    metal(const vec3& a) : albedo(a) {}
    
    virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        const vec3 reflectedRay = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = ray(rec.p, reflectedRay);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
    vec3 albedo;
};

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            //         cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx);
        else
            reflect_prob = 1.0;
        if (drand48() < reflect_prob)
            scattered = ray(rec.p, reflected);
        else
            scattered = ray(rec.p, refracted);
        return true;
    }
    float ref_idx;
};



#endif /* material_h */
