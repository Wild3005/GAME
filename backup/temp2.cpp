#include "raylib.h"
#include <iostream>

int main() {
    // Inisialisasi window
    InitWindow(800, 600, "IsKeyPressed vs IsKeyDown");
    bool temp = false;

    // Program berjalan selama window tidak ditutup
    while (!WindowShouldClose()) {
        // Mulai menggambar frame
        BeginDrawing();
        ClearBackground(RAYWHITE);


        // Jika tombol SPACE ditekan (hanya sekali per tekan)
        if (IsKeyPressed(KEY_Q)) {
            std::cout << "SPACE key pressed\n=====" << std::endl;
            DrawText("SPACE key PRESSED!", 200, 250, 20, GREEN);
        }

        // Jika tombol SPACE ditekan terus-menerus
        if (IsKeyDown(KEY_SPACE)) {
            std::cout << "SPACE key is being held down" << std::endl;
            DrawText("SPACE key is HELD DOWN!", 200, 300, 20, RED);
        }

        if(IsKeyPressed(KEY_Q)){
            temp = true;
        }else if(IsKeyPressedRepeat(KEY_Q)){
            temp = false;
        }

        std::cout << temp << std::endl;

        if (temp && IsKeyDown(KEY_W)) {
            std::cout << "gacorrrr" << std::endl;
            DrawText("GACORRR", 200, 300, 20, RED);
        }


        DrawText("Press and hold SPACE to see the difference!", 150, 400, 20, DARKGRAY);

        // Selesai menggambar frame
        EndDrawing();
    }

    // Tutup window dan bersihkan resource
    CloseWindow();

    return 0;
}
