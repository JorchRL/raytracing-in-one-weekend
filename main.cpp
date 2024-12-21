#include <iostream>
#include "Framebuffer.h"
#include <raylib.h>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"

int main() {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 20;

    cam.initialize();

    Framebuffer framebuffer(cam.image_width, cam.image_width/cam.aspect_ratio);

    std::thread render_thread([&cam, &world] ()
    {
    cam.render(world);
    });

    framebuffer.Run(cam.pixels, cam.pixel_mutex);
    render_thread.join();

    free(cam.pixels);

    return 0;
}
