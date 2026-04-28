#include "raylib.h"
#include <iostream>
#include <chrono>

float coldown = 180.0f;
int main() {
    // Inisialisasi window
    InitWindow(800, 600, "GetFrameTime Example");

    SetTargetFPS(60);  // Targetkan 60 FPS

    while (!WindowShouldClose()) {  // Loop utama
        // Mendapatkan waktu antar frame
        if(coldown <= 0 && IsKeyPressed(KEY_W)){
            coldown = 5.0f;
        }

        // if(coldown > 0 ) coldown -= GetFrameTime();
        if (coldown > 0.0f) {
            coldown -= GetFrameTime();
            if (coldown < 0.0f) coldown = 0.0f;  // Mencegah nilai negatif
        }
        // Cetak waktu antar frame di terminal
        // std::cout << "Delta Time: " << coldown << " seconds" << std::endl;
        std::cout << "Deuration : " << coldown << " seconds" << std::endl;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("Cooldown Tembak: %.2f", coldown), 10, 500, 20, BLACK);
        DrawText("Check terminal for GetFrameTime output", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
