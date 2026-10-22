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

//All the opcode functions

//Clears display
void op_00E0(Chip8 *c);

//Return from subroutine by poping from the stack and setting pc to the top
void op_00EE(Chip8 *c);

//Sets the program counter to addres NNN
void op_1NNN(Chip8 *c);

//Pushes the current PC onto the stack then gose into a subroutine by setting PC to NNN
void op_2NNN(Chip8 *c);

//Skips next instruction if register x == kk (which is a byte) a if statment
void op_3XKK(Chip8 *c);

//Skips next instruction if register x != kk 
void op_4XKK(Chip8 *c);

//Skips next instruction if register x == register k
void op_5XY0(Chip8 *c);

//Sets register x to kk
void op_6XKK(Chip8 *c);

//add kk to the value of register x, then set register x to that sum
void op_7XKK(Chip8 *c);
#endif
