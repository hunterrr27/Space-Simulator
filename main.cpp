#include <iostream>
#include "raylib.h"
#include "rlgl.h"

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Space Simulator");
    SetTargetFPS(60);

    // 1. Setup Camera
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f }; // Start back a bit
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };   // Look toward the center
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();

    Texture2D stars = LoadTexture("assets/stars.png");
    if (stars.id == 0) {
        std::cout << "--- ERROR ---" << std::endl;
        std::cout << "Could not find: " << GetWorkingDirectory() << "/assets/stars.png" << std::endl;
        std::cout << "--- ERROR ---" << std::endl;
    }

    while (!WindowShouldClose()) {
        // Update
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                // --- DRAW WORLD ---
                DrawTexture(stars, (Vector3){0.0f, 2.0f, 100.0f}, 100.0f, WHITE);
                DrawGrid(100, 1.0f);
                DrawCube((Vector3){0, 0.5f, 0}, 1.0f, 1.0f, 1.0f, RED);
            EndMode3D();
            DrawFPS(10, 10);
            DrawText("WASD to Move | Mouse to Look", 10, 30, 20, WHITE);
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}