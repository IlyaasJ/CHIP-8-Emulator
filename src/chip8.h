#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef struct {
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t index_register;
    uint16_t program_counter;
    uint8_t stack_pointer;
    uint16_t stack[16];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad;
    uint8_t video[64 * 32];
    uint16_t opcode;
    
} Chip8;

void chip8_init(Chip8 *c); //initializes the struct in real computer memory

int chip8_load(Chip8 *c, const char *filename); //Loads the rom data into chip8 emulated memory

uint8_t chip8_randGen(Chip8 *c); //Generates a random byte

#endif
