//
// Created by Jorge on 12/16/2024.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <raylib.h>

class Framebuffer {
public:
    Framebuffer(int width, int height);

    ~Framebuffer();

    void UpdatePixels(Color * newPixels);

    void Render();

    void Run();

private:
    int screenWidth;
    int screenHeight;
    Color *pixels;
    Image img;
    Texture2D tex;
};

#endif //FRAMEBUFFER_H
