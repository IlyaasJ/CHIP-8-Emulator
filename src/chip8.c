#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <stdio.h>
#include "chip8.h"

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

    // Main table (indexed by first digit)
    c->table[0x0] = Table0;
    c->table[0x1] = op_1NNN;
    c->table[0x2] = op_2NNN;
    c->table[0x3] = op_3XKK;
    c->table[0x4] = op_4XKK;
    c->table[0x5] = op_5XY0;
    c->table[0x6] = op_6XKK;
    c->table[0x7] = op_7XKK;
    c->table[0x8] = Table8;
    c->table[0x9] = op_9XY0;
    c->table[0xA] = op_ANNN;
    c->table[0xB] = op_BNNN;
    c->table[0xC] = op_CXKK;
    c->table[0xD] = op_DXYN;
    c->table[0xE] = TableE;
    c->table[0xF] = TableF;

    // Fill every sub-table slot with the no-op first
    for (size_t i = 0; i < 16; ++i) {
	c->table0[i] = op_NULL;
	c->table8[i] = op_NULL;
	c->tableE[i] = op_NULL;
    }
    for (size_t i = 0; i < 256; ++i) {
	c->tableF[i] = op_NULL;
    }

    // Then overwrite the real entries
    c->table0[0x0] = op_00E0;
    c->table0[0xE] = op_00EE;

    c->table8[0x0] = op_8XY0;
    c->table8[0x1] = op_8XY1;
    c->table8[0x2] = op_8XY2;
    c->table8[0x3] = op_8XY3;
    c->table8[0x4] = op_8XY4;
    c->table8[0x5] = op_8XY5;
    c->table8[0x6] = op_8XY6;
    c->table8[0x7] = op_8XY7;
    c->table8[0xE] = op_8XYE;

    c->tableE[0x1] = op_EXA1;
    c->tableE[0xE] = op_EX9E;

    c->tableF[0x07] = op_FX07;
    c->tableF[0x0A] = op_FX0A;
    c->tableF[0x15] = op_FX15;
    c->tableF[0x18] = op_FX18;
    c->tableF[0x1E] = op_FX1E;
    c->tableF[0x29] = op_FX29;
    c->tableF[0x33] = op_FX33;
    c->tableF[0x55] = op_FX55;
    c->tableF[0x65] = op_FX65; 
}

uint8_t chip8_randByte(void){
    return (rand() % (256));
}

void chip8_cycle(Chip8 *c){
    // Fetch
    c->opcode = (c->memory[c->program_counter] << 8) | (c->memory[c->program_counter + 1]);

    //Move
    c->program_counter += 2;

    //Decode
    c->table[(c->opcode & 0xF000) >> 12](c);

    if (c->delay_timer){
	c->delay_timer -= 1;
    }

    if (c->sound_timer){
	c->sound_timer -= 1;
    }
}

int chip8_load(Chip8 *c, const char *filename){
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) return 0;
	

    size_t n = fread(&c->memory[START], 1, sizeof(c->memory) - START, fptr);
    fclose(fptr);

    return n > 0;
}

//Table functions
void Table0(Chip8 *c){
    c->table0[c->opcode & 0x000F](c);
}

void Table8(Chip8 *c){
    c->table8[c->opcode & 0x000F](c);
}

void TableE(Chip8 *c){
    c->tableE[c->opcode & 0x000F](c);
}

void TableF(Chip8 *c){
    c->tableF[c->opcode & 0x00FF](c);
}


//opcode definitions
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

void op_8XY0(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    c->registers[x] = c->registers[y];
}

void op_8XY1(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;
    
    c->registers[x] |= c->registers[y];
}

void op_8XY2(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;
    
    c->registers[x] &= c->registers[y];
}

void op_8XY3(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;
    
    c->registers[x] ^= c->registers[y];
}

void op_8XY4(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;
    
    uint16_t sum = c->registers[x] + c->registers[y];

    c->registers[x] = sum & 0xFF;
    c->registers[0xF] = (sum > 0xFF);
}

void op_8XY5(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    uint8_t Vx = c->registers[x];
    uint8_t Vy = c->registers[y];

    c->registers[x] = (Vx - Vy); 
    c->registers[0xF] = (Vx >= Vy);
}

void op_8XY6(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;

    uint8_t Vx = c->registers[x];


    c->registers[x] = Vx >> 1;
    c->registers[0xF] = (Vx & 0x1);
}

void op_8XY7(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    uint8_t Vx = c->registers[x];
    uint8_t Vy = c->registers[y];

    c->registers[x] = Vy - Vx;
    c->registers[0xF] = (Vy >= Vx);
}


void op_8XYE(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;

    uint8_t Vx = c->registers[x];

    c->registers[x] = Vx << 1;
    c->registers[0xF] = (Vx & 0x80) >> 7;
}

void op_9XY0(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    uint8_t Vx = c->registers[x];
    uint8_t Vy = c->registers[y];

    if (Vx != Vy) {
	c->program_counter += 2;
    }
}

void op_ANNN(Chip8 *c){
    uint16_t address = (c->opcode & 0x0FFF);

    c->index_register = address;
}

void op_BNNN(Chip8 *c){
    uint16_t address = (c->opcode & 0x0FFF);
    uint8_t V0 = c->registers[0];

    c->program_counter = (address + V0);
}

void op_CXKK(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t kk = (c->opcode & 0x00FF);

    c->registers[x] = (chip8_randByte() & kk);
}

void op_DXYN(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t y = (c->opcode & 0x00F0) >> 4;

    uint8_t xPos = c->registers[x] % 64;
    uint8_t yPos = c->registers[y] % 32; //TODO: Define screen height and width above
    uint8_t totalRows = (c->opcode & 0x000F);
    
    c->registers[0xF] = 0;

    for (unsigned int row = 0; row < totalRows; ++row){
	if (yPos + row >= 32) break;
        uint8_t spriteByte = c->memory[c->index_register + row];
        
        for (unsigned int col = 0; col < 8; ++col){
	    if (xPos + col >= 64) break;
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            uint8_t *screenPixel = &c->video[(yPos + row) * 64 + (xPos + col)];

            if (spritePixel){
                if(*screenPixel){
                    c->registers[0xF] = 1;
                }

                *screenPixel ^= (0xFF);
            }
        }
    }
}

void op_EX9E(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t key = c->registers[x] & 0xF;

    if(c->keypad[key]){
	c->program_counter += 2;
    }
}

void op_EXA1(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t key = c->registers[x] & 0xF;

    if(!c->keypad[key]){
	c->program_counter += 2;
    }
}

void op_FX07(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    c->registers[x] = c->delay_timer;
}

void op_FX0A(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    
    int pressed = -1;
    for(unsigned int i=0; i<16; ++i){
	if(c->keypad[i]){ 
	    pressed = i;
	    break;
	}
    }

    if (pressed >= 0){ 
	c->registers[x] = pressed;
    } else
    {
	c->program_counter -= 2;
    }
}

void op_FX15(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;

    c->delay_timer = c->registers[x];
}

void op_FX18(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;

    c->sound_timer = c->registers[x];
}

void op_FX1E(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;

    c->index_register += c->registers[x];
}

void op_FX29(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t digit = c->registers[x] & 0xF;

    c->index_register = FONTSET_START + (5 * digit); 
}

void op_FX33(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    uint8_t value = c->registers[x];

    //Ones
    c->memory[c->index_register + 2] = value % 10;
    value /= 10;

    //Tens
    c->memory[c->index_register + 1] = value % 10;
    value /= 10;

    //Hundreds
    c->memory[c->index_register] = value % 10;
}

void op_FX55(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    
    for(unsigned int i=0; i <= x; ++i){
	c->memory[c->index_register + i] = c->registers[i];
    }
}

void op_FX65(Chip8 *c){
    uint8_t x = (c->opcode & 0x0F00) >> 8;
    
    for(unsigned int i=0; i <= x; ++i){
	 c->registers[i] = c->memory[c->index_register + i];
    } 
}


