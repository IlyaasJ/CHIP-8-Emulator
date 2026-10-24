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
    uint8_t keypad[16];
    uint8_t video[64 * 32];
    uint16_t opcode;
    
} Chip8;

void chip8_init(Chip8 *c); //initializes the struct in real computer memory

int chip8_load(Chip8 *c, const char *filename); //Loads the rom data into chip8 emulated memory

uint8_t chip8_randByte(void); //Generates a random byte

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

//Sets register x to what is in register y
void op_8XY0(Chip8 *c);

//Performs an OR operation on register x and register y then set the value to register x
void op_8XY1(Chip8 *c);

//Performs an AND operation between reigster x and register y then sets the value to register x
void op_8XY2(Chip8 *c);

//Performs a XOR operation between Vx and Vy and sets it to Vx
void op_8XY3(Chip8 *c);

//Performs ADD operattion between Vx and Vy, however, if it overflows 8 bits, VF is set to 1, other wise VF is set to 0 
void op_8XY4(Chip8 *c);

//Performs Vx > Vy, if true, VF = 1, else 0 and set Vx to Vx - Vy
void op_8XY5(Chip8 *c);

//If the LSB in Vx is set to VF, then Vx is cut in half (Vx >> 1)
void op_8XY6(Chip8 *c);

//Set VF to 1 if Vy >= Vx, then set Vx to Vy - Vx
void op_8XY7(Chip8 *c);

//If the MSB in Vx is set ot VF, than Vx is mutiplied by 2
void op_8XYE(Chip8 *c);

//If Vx != Vy, skip next intruction
void op_9XY0(Chip8 *c);

//Set index register to nnn
void op_ANNN(Chip8 *c);

//program counter is set to nnn plus value of V0
void op_BNNN(Chip8 *c);

//a random byte is AND'ed with the bytes at kk and set to Vx
void op_CXKK(Chip8 *c);

//Displays sprite(n bytes of memory past index register, n rows, 8 columns for sprite) at Vx, Vy
//Sprite is XOR'ed onto screen, Sets VF flag if pixel is erased during XOR, wraps sprites back if
//out of bounds
void op_DXYN(Chip8 *c);

#endif
