#include "chip8.h"
#include <string.h>
#include <stdio.h>

#define START 0x200 // Startng address in chip8 emulated memory

void chip8_init(Chip8 *c){
    memset(c, 0, sizeof(*c));
    c->program_counter = START;
}

int chip8_load(Chip8 *c, const char *filename){
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) return 0;
	

    size_t n = fread(&c->memory[START], 1, sizeof(c->memory) - START, fptr);
    fclose(fptr);

    return n > 0;
}
