#include "chip8.h"
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include "raylib.h"

//This is boilerplate code for now while the chip8 is being devloped
int main(void) {
    SetTraceLogLevel(LOG_ERROR); 
    Chip8 newChip8;
    printf("Hello");
    //chip8_init(&newChip8);


    InitWindow(500, 500, "raylib window");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("raylib works!", 250, 250, 30, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
