//
// Created by Jorge on 12/16/2024.
//

#include "Framebuffer.h"

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
    UnloadTexture(tex);
    UnloadImage(img);
    CloseWindow();
}

void Framebuffer::UpdatePixels(Color * newPixels) {
    img.data = newPixels;
    UpdateTexture(tex, img.data);
}

void Framebuffer::Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // UpdateTexture(tex, pixels);
    DrawTexture(tex, 0, 0, WHITE);
    EndDrawing();
}

void Framebuffer::Run() {

    while (!WindowShouldClose()) {
        Render();
    }
}
