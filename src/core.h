#ifndef CORE_H_
#define CORE_H_

#include "types.h"
#include "timer.h"
#include "screen.h"


#define CLOCK_SPEED         500
#define SAVE_SLOTS          4
#define STACK_SIZE          16
#define MEMORY_SIZE         0x1000
#define PROGRAM_BASE_ADDR   0x200
#define FONT_BASE_ADDR      0x0

typedef byte_t charset_t[16][5];
typedef word_t instruction_t;

typedef struct
{
    word_t PC;               // Program Counter
    byte_t Mem[MEMORY_SIZE]; // 4kB memory
    byte_t V[16];            // 16 8-bits registers
    word_t I;                // Memory pointer
    mstimer_t delay_timer;
    mstimer_t sound_timer;
    screen_t screen;
    word_t stack[STACK_SIZE];
    word_t *SP;
} State;

int Core_Init(char* rom_path);
void Core_Reset();
void Core_SkipInstr();
instruction_t Core_ReadOpcode();
void Core_ExecuteInstr(instruction_t instr);
void Core_SaveState();
void Core_LoadState();
void Core_Exit();

#endif /* CORE_H_ */
