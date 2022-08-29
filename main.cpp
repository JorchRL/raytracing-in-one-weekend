#include "common/raytracer.h"
#include "common/color.h"
#include "common/camera.h"

#include "geometry/hittable_list.h"
#include "geometry/sphere.h"

#include <iostream>


color ray_color(const ray& r, const hittable& world, int bounces) {

  hit_record rec;

  if (bounces <= 0) {
    return color(0, 0, 0);
  }

  if (world.hit(r, 0, infinity, rec)) {
    point3 target = rec.p + rec.normal + random_in_unit_sphere();
    // return 0.5 * (rec.normal + color(1, 1, 1));
    ray child = ray(rec.p, target);
    return 0.5 * ray_color(child, world, bounces - 1);
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  const int samples_per_pixel = 2;
  const int bounces = 50;

  // world
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(1, 0, -1), 0.2));
  world.add(make_shared<sphere>(point3(-1, 0, -2), 1));

  camera cam;

  // rendering
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    for (int i = 0; i < image_width; ++i) {

      color pixel_color(0.0, 0.0, 0.0);
      for (int s = 0; s < samples_per_pixel; ++s) {

        // "scan" the scene by tracing a ray
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        ray r = cam.get_ray(u, v);

        // write final pixel colors
        pixel_color += ray_color(r, world, bounces);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
}