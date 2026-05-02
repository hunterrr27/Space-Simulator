#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

class Body {
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;
    float inverseMass;
    float radius;
    Color color;

    Body(Vector3 pos, Vector3 vel, float m, float r, Color c) : position(pos), velocity(vel), mass(m), radius(r), color(c) {
        if (m > 0.0f) {
            inverseMass = 1.0f / m;
        } else {
            inverseMass = 0.0f;
        }
    }

    void Update(float dt) {
        position = Vector3Add(position, Vector3Scale(velocity, dt));
    }
    void ApplyGravity(Body& other, float G, float dt) {
        Vector3 direction = Vector3Subtract(other.position, position);
        float distanceSq = Vector3LengthSqr(direction);
        if (distanceSq < 0.1f) return;
        float forceMagnitude = G * (mass * other.mass) / distanceSq;
        Vector3 force = Vector3Scale(Vector3Normalize(direction), forceMagnitude);
        Vector3 accelerationLocal = Vector3Scale(force, inverseMass);
        velocity = Vector3Add(velocity, Vector3Scale(accelerationLocal, dt));
    }
    void Draw() {
        DrawSphere(position, radius, color);
    }
};

class Space {
public:
    std::vector<Body> bodies;

    float G = 1.0f;
    float timeScale = 1.0f;

    void Update(float dt) {
        float fixedDt = dt * timeScale;

        for (size_t i = 0; i < bodies.size(); i++) {
            for (size_t j = 0; j < bodies.size(); j++) {
                if (i == j) continue;
                bodies[i].ApplyGravity(bodies[j], G, fixedDt);
            }
        }
        for (auto& b : bodies) {
            b.Update(fixedDt);
        }
    }
};

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    Body* selectedBody = nullptr;

    InitWindow(screenWidth, screenHeight, "Space Simulator");
    SetTargetFPS(60);

    // 1. Setup Camera
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 50.0f, 100.0f }; 
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };   
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Space space;
    space.bodies.emplace_back(Body({0, 0, 0}, {0, 0, 0}, 1000.0f, 5.0f, YELLOW));
    space.bodies.emplace_back(Body({40, 0, 0}, {0, 0, 5}, 1.0f, 1.5f, BLUE));

    DisableCursor();
    while (!WindowShouldClose()) {
        // Update
        UpdateCamera(&camera, CAMERA_FREE);
        space.Update(GetFrameTime());

        /* Unlock cursor when c is pressed so user can use sliders
        if (IsKeyPressed(KEY_C)) {
            EnableCursor();
            UpdateCamera(&camera, CAMERA_LOCK);
        }
        */

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                // --- DRAW WORLD ---
                DrawGrid(20.0f, 10.0f);
                for (auto& b : space.bodies) {
                    b.Draw();
                    DrawLine3D(b.position, b.velocity, RED);
                    
                }
            EndMode3D();
            //space.G = GuiSlider((Rectangle){60,120,200,20}, "G Force", NULL, &space.G, 0.0f, 10.0f);
            //space.timeScale = GuiSlider((Rectangle){60,150,200,20}, "Time Scale", NULL, &space.timeScale, 0.0f, 10.0f);
            DrawFPS(1100.0f, 20.0f);
            DrawText("WASD to Move | Mouse to Look", 10, 30, 20, WHITE);

            // 1. Check for clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Ray ray = GetMouseRay(GetMousePosition(), camera);
                bool hitAnything = false;
                
                for (auto& b : space.bodies) {
                    RayCollision collision = GetRayCollisionSphere(ray, b.position, b.radius);
                    if (collision.hit) {
                        selectedBody = &b;
                        hitAnything = true;
                        break;
                    }
                }
                if (!hitAnything) selectedBody = nullptr;
            }

            // 2. Draw UI for selection
            if (selectedBody != nullptr) {
                DrawText("Body Selected", 60, 100, 20, GREEN);
                
                // Pass the pointer to the mass variable so the slider can modify it directly
                GuiSlider((Rectangle){60, 180, 200, 20}, "MASS", NULL, &selectedBody->mass, 0.1f, 5000.0f);
                
                // Smoothly follow the planet
                camera.target = selectedBody->position;
            }
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}