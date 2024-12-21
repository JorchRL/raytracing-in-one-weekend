//
// Created by Jorge on 12/16/2024.
//

#include "Framebuffer.h"

#include <iostream>
#include <mutex>
#include <ostream>
#include <stdlib.h>


Framebuffer::Framebuffer(int width, int height): screenWidth(width),
                                                 screenHeight(height) {
    InitWindow(screenWidth, screenHeight, "Hello World");
    pixels = (Color *) malloc(screenWidth * screenHeight * sizeof(Color));
    img = {
        .data = pixels,
        .width = screenWidth,
        .height = screenHeight,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };
    tex = LoadTextureFromImage(img);
    SetTargetFPS(60);
}

Framebuffer::~Framebuffer() {
    img.data = pixels;
    UnloadTexture(tex);
    UnloadImage(img);
    CloseWindow();
    // if (pixels) free(pixels);
}

void Framebuffer::UpdatePixels(Color * newPixels) {
    img.data = newPixels;
}

void Framebuffer::Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateTexture(tex, img.data);
    DrawTexture(tex, 0, 0, WHITE);
    // display fps

    DrawFPS(10, 10);

    EndDrawing();
}

void Framebuffer::Run(Color * sharedPixels, std::mutex& pixelMutex) {

    while (!WindowShouldClose()) {
        {
            std::lock_guard<std::mutex> lock(pixelMutex);
            UpdatePixels(sharedPixels);
        }
        Render();
    }
    std::cout << "Exiting" << std::endl;
}
