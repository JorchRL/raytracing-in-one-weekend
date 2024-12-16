#include <iostream>
#include "Framebuffer.h"
#include <raylib.h>
#include <stdlib.h>


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    Framebuffer framebuffer(screenWidth, screenHeight);

    Color *pixels = (Color *) malloc(screenWidth * screenHeight * sizeof(Color));

    for (int x = 0; x < screenWidth; x++) {
        for (int y = 0; y < screenHeight; y++) {
            auto r = double(x) / (screenWidth-1);
            auto g = double(y) / (screenHeight-1);
            auto b = 0.25;
            Color color = {
                .r = static_cast<unsigned char>(255.999 * r),
                .g = static_cast<unsigned char>(255.999 * g),
                .b = static_cast<unsigned char>(255.999 * b),
                .a = 255
            };
            pixels[y * screenWidth + x] = color;
        }
    }

    framebuffer.UpdatePixels(pixels);
    framebuffer.Run();

    return 0;
}
