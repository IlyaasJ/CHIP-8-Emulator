#include <stdio.h>
#include "raylib.h"
#include "chip8.h"

#define video_scale 15
#define cycles_per_frame 15

int main(void) {
    Chip8 emulator;
    chip8_init(&emulator);
    if (!chip8_load(&emulator, "../ROMS/Tic-Tac-Toe.ch8")) {
       printf("Failed to load ROM\n");
       return 1;
    }

    InitWindow(64 * video_scale, 32 * video_scale, "CHIP-8");
    SetTargetFPS(60);

    //Drawing from video buffer
    Image img = GenImageColor(64, 32, BLACK);
    Texture2D tex = LoadTextureFromImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    UnloadImage(img);   // the CPU copy isn't needed once it's on the GPU
	

    while (!WindowShouldClose()) {

	//Input
	emulator.keypad[0x1] = IsKeyDown(KEY_ONE);
	emulator.keypad[0x2] = IsKeyDown(KEY_TWO);
	emulator.keypad[0x3] = IsKeyDown(KEY_THREE);
	emulator.keypad[0xC] = IsKeyDown(KEY_FOUR);
	emulator.keypad[0x4] = IsKeyDown(KEY_Q);
	emulator.keypad[0x5] = IsKeyDown(KEY_W);
	emulator.keypad[0x6] = IsKeyDown(KEY_E);
	emulator.keypad[0xD] = IsKeyDown(KEY_R);
	emulator.keypad[0x7] = IsKeyDown(KEY_A);
	emulator.keypad[0x8] = IsKeyDown(KEY_S);
	emulator.keypad[0x9] = IsKeyDown(KEY_D);
	emulator.keypad[0xE] = IsKeyDown(KEY_F);
	emulator.keypad[0xA] = IsKeyDown(KEY_Z);
	emulator.keypad[0x0] = IsKeyDown(KEY_X);
	emulator.keypad[0xB] = IsKeyDown(KEY_C);
	emulator.keypad[0xF] = IsKeyDown(KEY_V);

	//Cycle
	for(int i=0; i<cycles_per_frame; i++)
	    chip8_cycle(&emulator);

	//Timers (once per frame, 60Hz)
	if (emulator.delay_timer > 0) emulator.delay_timer--;
	if (emulator.sound_timer > 0) emulator.sound_timer--;

        BeginDrawing();
        ClearBackground(BLACK);

	
	uint32_t pixels[64 * 32];
	for(int i=0; i< 64 * 32; i++)
	    pixels[i] = emulator.video[i] ? 0xFFFFFFFF : 0xFF000000;

	UpdateTexture(tex, pixels);
	DrawTexturePro(tex,
                (Rectangle){0, 0, 64, 32},                  // source: the whole texture
                (Rectangle){0, 0, 64 * video_scale, 32 * video_scale},  // destination: the size on screen
                (Vector2){0, 0},                            // origin (rotation pivot)
                0.0f,                                       // rotation
                WHITE);                                     // tint, WHITE means unchanged

        EndDrawing();
    }

    UnloadTexture(tex);
    CloseWindow();
    return 0;
}
