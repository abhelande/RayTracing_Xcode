//
//  main.cpp
//  RayTracingInAWeekend
//
//  Created by Abhijit Bhelande on 6/25/19.
//  Copyright © 2018 Abhijit Bhelande. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <random>
#include "hitable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "camera.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define OUTPUT_DEBUG_GRADIENT 0
constexpr uint32_t maxBounces = 50;
constexpr uint32_t nPixelSamples = 200;
constexpr char outputFormat[] = ".bmp";

// simple 4 tupule struct to represent pixel of final image plane
// RGBA channel ordering
class PixelRGBA {
public:
    uint8_t r, g, b, a;
    
    PixelRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
    PixelRGBA() { PixelRGBA(0, 0, 0, 0); }
    static int getNumComponents() { return 4; }
};

// A snapshot is simply an image of the world taken from a certain angle
// Also a label to identify the shot
class snapshot {
public:
    camera cam;
    std::string label;
    
    snapshot() = delete;
    snapshot(camera c,
             const char* id) :  cam(c),
                                label(id) {}
};

// Global image parameters
const int outImageWidth = 400;
const int outImageHeight = 200;
const float aspect = (float)outImageWidth / (float)outImageHeight;

// Debug:
// Render a gradient - this is just to get a sense of orientation
// of the image format and type
// This will tell you if image orientation is top left or bottome left.
void RGGradientInto(PixelRGBA *col)
{
    for (int j = outImageHeight - 1; j >= 0; j--) {
        for (int i = 0; i < outImageWidth; i++) {
            
            float u = (float)i / (float)outImageWidth;
            float v = (float)j / (float)outImageHeight;
            
            PixelRGBA *p = &col[(outImageHeight - j - 1) * outImageWidth + i];
            p->r = int(u * 255.99);
            p->g = int(v * 255.99);
            p->b = 0;
            p->a = 255;
        }
    }
}

// Background color
// If ray hits infinity without hitting any object, return this
// background is a light blue gradient
vec3 bgColorAtRay(const ray& r)
{
    // This returns lerped blue fade
    vec3 unitDir = unit_vector(r.direction());
    float t = 0.5f * (unitDir.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

// Return color at Ray
// for each intersection, gather color for material at point of intersection
// and any subsequent refelected / refracted attenuated ray
// Do this no more than bounceDepth times per ray
//
// If it hits nothing - return bg color
vec3 colorAtRay(const ray& r,
                scene& world,
                uint32_t bounceDepth)
{
    intersectParams rec;
    if (world.hit(r, 0.0001f, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (bounceDepth < maxBounces &&
            rec.surfaceMat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * colorAtRay(scattered, world, bounceDepth + 1);
        } else {
            // shadow color
            return vec3(0.0f);
        }
    }
    
    return bgColorAtRay(r);
}

// For a given camera / scene - do ray trace
// and gsther collected samples into RGBA destination buffer
// Fires 'nPixelSamples' offset randomly per pixel.
// and applies gamma correction
void traceInto(PixelRGBA *rgbaTarget,
               scene& world,
               camera& cam)
{
    // use std uniform random distribution to generate
    // ray offsets. Not the best option, but it's a start
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distr;

    for (int j = outImageHeight - 1; j >= 0; j--) {
        for (int i = 0; i < outImageWidth; i++) {
            vec3 gather(0, 0, 0);
            for (uint32_t s = 0; s < nPixelSamples; s++) {
                float u = (float(i) + distr(gen)) / float(outImageWidth);
                float v = (float(j) + distr(gen)) / float(outImageHeight);
                ray r = cam.getRayAt(u, v);
                gather += colorAtRay(r, world, 0);
            }

            vec3 col = gather / float(nPixelSamples);
            // gamma correction
            constexpr float gamma = 1.0f / 2.2f;
            col = vec3(pow(col[0], gamma), pow(col[1], gamma), pow(col[2], gamma));
            // convert [0, 1] -> [0, 255] ranges for rgb
            int ir = int(255.99f * col[0]);
            int ig = int(255.99f * col[1]);
            int ib = int(255.99f * col[2]);
            
            // write to image target;
            PixelRGBA *p = &rgbaTarget[(outImageHeight - j - 1) * outImageWidth + i];
            p->r = ir;
            p->g = ig;
            p->b = ib;
            p->a = 255;
        }
    }
}

// Create scene data
void generateScene(scene &world)
{
    // hovering triangle
    world.objects.emplace_back(new triangle(vec3(-3.0f, 0.0f, -3.0f),
                                            vec3( 3.0f, 1.0f, -2.0f),
                                            vec3(-2.0f, 2.0f, -1.5f),
                                            new metal(vec3(0.8, 0.1, 0.5))));
    // center metaalic sphere
    world.objects.emplace_back(new sphere(vec3(0.0f, 0.0f, -1.0f),
                                          0.5f,
                                          new metal(vec3(0.1, 0.2, 0.5))));
    // left refracting sphere
    world.objects.emplace_back(new sphere(vec3(-1.0f, 0.0f, -1.0f),
                                          0.5f,
                                          new dielectric(1.5)));
    // right back fuzzy metallic silver
    world.objects.emplace_back(new sphere(vec3(1.0f, 0.0f, -2.0f),
                                          0.5f,
                                          new lambertian(vec3(0.8, 0.8, 0.8))));
    {
        // checkerboard hovering sphere
        flatShade* shade0 = new flatShade(vec3(0.9, 0.5, 0.0f));
        flatShade* shade1 = new flatShade(vec3(0.9, 0.9, 0.9));
        checkerBoard* checkTex = new checkerBoard(shade0, shade1);
        world.objects.emplace_back(new sphere(vec3(3.0f, 2.0f, -3.0f),
                                              1.5f,
                                              new lambertianTexture(checkTex)));
    }
    
    // Green white patterned base
    {
        flatShade* shade0 = new flatShade(vec3(0.2, 0.3, 0.1));
        flatShade* shade1 = new flatShade(vec3(0.9, 0.9, 0.9));
        checkerBoard* checkTex = new checkerBoard(shade0, shade1);
        world.objects.emplace_back(new sphere(vec3(0.0f, -100.5f, -1.0f),
                                              100.0f,
                                              new lambertianTexture(checkTex)));
    }
}

int main(int argc, const char * argv[]) {
    
    int nx = outImageWidth;
    int ny = outImageHeight;

#if OUTPUT_DEBUG_GRADIENT
    {
        // Debug:
        // Render a gradient - this is just to get a sense of orientation
        // of the image format and type
        // This will tell you if image orientation is top left or bottome left.
        PixelRGBA grad[nx * ny];
        RGGradientInto(grad);
        unsigned char *data = (unsigned char *)grad;
        stbi_write_bmp("out_image_grad.bmp", outImageWidth, outImageHeight, 4, data);
    }
#endif
    
    {
        // create world
        scene world;
        generateScene(world);
        
        // trace
        PixelRGBA col[nx * ny];
        // Each snapshot corresponds to some camera view of the world / scene
        std::vector<snapshot> snapshots = {
            snapshot(camera(50.0f,
                            aspect),
                     "RayTrace_Image_1"),
            
            snapshot(camera(15.0f,
                            aspect,
                            vec3(5.0f, 1.0f, 3.0f), // from
                            vec3(0.0f, 1.0f, -1.0f) // At
                     ),
                     "RayTrace_Image_2"),
        };
        
        // generate above snapshots of the scene
        fprintf(stderr, "\nTracing into %u x %u images, w %u samples per pixel.",
                                outImageWidth, outImageHeight, nPixelSamples);
        for (snapshot& snap : snapshots) {
            fprintf(stderr, "\n\nGenerating scene %s ... ", snap.label.c_str());
            traceInto(col, world, snap.cam);
            fprintf(stderr, "Done.");
            unsigned char *data = (unsigned char *)col;
            fprintf(stderr, "\nOutputting image ... ");
            stbi_write_bmp(snap.label.append(outputFormat).c_str(),
                           outImageWidth, outImageHeight,
                           PixelRGBA::getNumComponents(),
                           data);
            fprintf(stderr, "Done.");
        }
        fprintf(stderr, "\nAll Done!\n");
    }
    
    return 0;
}
