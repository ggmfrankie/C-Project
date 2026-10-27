//
// Created by ggmfrankie on 10/27/26.
//

#include <stdio.h>

#include "Dependencies/raylib/src/raylib.h"

int main() {
    InitWindow(800, 600, "Hello");
    SetTargetFPS(60);

    asm("xor %eax, %eax");

    Vector2 pos = {10, 10};

    Camera3D camera = {.fovy = 70, .projection = CAMERA_PERSPECTIVE};
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(10, 10);

        DrawRectangle(pos.x, pos.y, 100, 50, RED);
        DrawText("Hello from Raylib", pos.x+4, pos.y+4, 4, BLACK);

        char buff[512];
        sprintf(buff, "Elapsed time: %.2lf", GetTime());
        DrawText(buff, GetScreenWidth() - 500, 20, 30, RED);
        const Vector2 spline[] = {
            {0,0},
            {10, 10},
            {20, 30},
            {30, 35},
            {40, 70},
            {500, 400}
        };
        constexpr int size = sizeof(spline) / sizeof(*spline);
        DrawSplineLinear(spline, size, GetRandomValue(5, 30), GRAY);

        DrawLine(0,0, GetScreenWidth(), GetScreenHeight(), BLUE);
        //if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();

        if (IsKeyDown(KEY_W)) pos.y -= 3;
        if (IsKeyDown(KEY_S)) pos.y += 3;
        if (IsKeyDown(KEY_A)) pos.x -= 3;
        if (IsKeyDown(KEY_D)) pos.x += 3;

        BeginMode3D(camera);
            DrawCube((Vector3){0, 0, 120}, 100, 100, 100, GRAY);
        EndMode3D();

        EndDrawing();
    }
    return  0;
}
