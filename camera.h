//
// Created by Jorge on 12/18/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <thread>
#include <vector>

#include "Framebuffer.h"
#include "hittable.h"
#include "ray.h"
#include "raylib.h"
#include "vec3.h"

class camera
{
public:
    int image_width{};
    double aspect_ratio{};
    int samples_per_pixel = 10; // Number of random samples per pixel
    int max_depth = 10;  // Maximum number of rays bounces into the scene

    Color* pixels = nullptr;
    std::mutex pixel_mutex;


    void initialize()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_sample_scale = 1.0 / samples_per_pixel;

        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
        center = point3(0, 0, 0);

        // viewport
        const auto viewport_u = vec3(viewport_width, 0, 0);
        const auto viewport_v = vec3(0, -viewport_height, 0);
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        const auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


        // initialize the pixels array which will be shared with the framebuffer. Ensuring
        // that the pixels array is initialized before the threads are spawned. And before
        // the framebuffer tries to access it!
        if (pixels == nullptr)
        {
            pixels = static_cast<Color*>(malloc(image_width * image_height * sizeof(Color)));
        }
    }

    void render(const hittable& world)
    {
        // initialize();
        // auto f = Framebuffer(image_width, image_height);
        // framebuffer = &f;

        // pixels = static_cast<Color*>(malloc(image_width * image_height * sizeof(Color)));

        const int num_threads = std::thread::hardware_concurrency();
        const int rows_per_thread = image_height / num_threads;

        std::vector<std::thread> threads;

        for (int thread_id = 0; thread_id < num_threads; ++thread_id)
        {
            int start_row = thread_id * rows_per_thread;
            int end_row = (thread_id == num_threads -1) ? image_height :
            start_row + rows_per_thread;

            threads.emplace_back(&camera::render_rows, this, std::ref(world), start_row, end_row);
        }

        for (auto& thread : threads)
        {
            thread.join();
        }

        // framebuffer->UpdatePixels(pixels);
        // framebuffer->Run();
    }

private:
    // Framebuffer* framebuffer = nullptr;

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_sample_scale = 0;
    point3 center;
    int image_height = 0;
    vec3 pixel00_loc;

    void render_rows(const hittable& world, int start_row , int end_row)
    {
        for (int y = start_row; y < end_row; y++) {
            for (int x = 0; x < image_width; x++) {
                color c = color(0, 0, 0);

                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(x, y);
                    c += ray_color(r, max_depth, world);
                }

                // "write_color" equivalent from the rtWeekend tutorial
                const Color pixelColor = vec3ColorToRaylibColor(pixel_sample_scale * c);
                std::lock_guard<std::mutex> lock(pixel_mutex);
                pixels[y * image_width + x] = pixelColor;
            }
        }
    }

    ray get_ray(int x, int y)
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc +
            ((x + offset.x()) * pixel_delta_u) +
            ((y + offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    static vec3 sample_square()
    {
        return {random_double() - 0.5, random_double() - 0.5, 0.0};
    }


    static color ray_color(const ray& r, int depth, const hittable& world)
    {
        if (depth <= 0)
            return color(0, 0, 0);

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec))
        {
            // bounce the ray from the surface in a random direction
            vec3 direction = rec.normal + random_unit_vector() ;
            return 0.5 * ray_color(ray(rec.p, direction), depth -1, world );
        }


        const vec3 unit_direction = unit_vector(r.direction());
        const auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    static double linear_to_gamma(double linear_component)
    {
        if (linear_component > 0 )
        {
            return std::sqrt(linear_component);
        }
        return 0;
    }

    static Color vec3ColorToRaylibColor(const color& vec3)
    {
        // Raylib uses 0-255 unsigned char (int) for color values
        // vec3 uses 0-1 (double) for color values
        static const interval intensity(0.000, 0.999);
        return {
            .r = static_cast<unsigned char>(256 * intensity.clamp(linear_to_gamma(vec3.x()))),
            .g = static_cast<unsigned char>(256 * intensity.clamp(linear_to_gamma(vec3.y()))),
            .b = static_cast<unsigned char>(256 * intensity.clamp(linear_to_gamma(vec3.z()))),
            .a = 255
        };
    }
};


#endif //CAMERA_H
