#include "chip8.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define START 0x200 // Startng address in chip8 emulated memory
#define FONTSET_START 0x50 // Starting address of font sprites
#define FONTSET_SIZE 80 // Size of fontset array in bytes

//All the characters, represented in hexadecimal
uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *c){
    memset(c, 0, sizeof(*c));
    c->program_counter = START;

    //loads the font sprites into memory
    for (unsigned int i = 0; i<FONTSET_SIZE; ++i){
        c->memory[FONTSET_START + i] = fontset[i];
    }

    srand((unsigned)time(NULL));
}

uint8_t chip8_randGen(Chip8 *c){
    return (rand() % (256));
}

int chip8_load(Chip8 *c, const char *filename){
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) return 0;
	

    size_t n = fread(&c->memory[START], 1, sizeof(c->memory) - START, fptr);
    fclose(fptr);

    return n > 0;
}

void op_00E0(Chip8 *c) {
    memset(c->video, 0, sizeof(c->video));
}

void op_00EE(Chip8 *c) {
    c->stack_pointer -= 1;
    c->program_counter = c->stack[c->stack_pointer];
}

void op_1NNN(Chip8 *c) {
   uint16_t address = c->opcode & 0x0FFF;
   c->program_counter = address;
}

void op_2NNN(Chip8 *c) {
    uint16_t address = c->opcode & 0x0FFF;

    c->stack[c->stack_pointer] = c->program_counter;
    c->stack_pointer += 1;

    c->program_counter = address;
}

void op_3XKK(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t kk = c->opcode & 0x00FF; 

    if (c->registers[x] == kk)
    {
	c->program_counter += 2;
    }
}

void op_4XKK(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t kk = c->opcode & 0x00FF; 

    if (c->registers[x] != kk)
    {
	c->program_counter += 2;
    }
}

void op_5XY0(Chip8 *c) {
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    if (c->registers[x] == c->registers[y])
    {
	c->program_counter += 2;
    }
}

void op_6XKK(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t kk = (c->opcode & 0x00FF);

    c->registers[x] = kk;
}

void op_7XKK(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t kk = (c->opcode & 0x00FF);

    //Once 255 is passed, it wraps back to 0, a design of chip8
    c->registers[x] += kk;
}



