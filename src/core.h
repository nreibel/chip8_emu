#ifndef CORE_H_
#define CORE_H_

#include "types.h"
#include "timer.h"

#define MEMORY_SIZE         0x1000
#define PROGRAM_BASE_ADDR   0x200
#define FONT_BASE_ADDR      0x0
#define FONTS_HEIGHT 		5

typedef byte_t charset_t[16][FONTS_HEIGHT];
typedef word_t instruction_t;

word_t 			PC;					// Program Counter
byte_t 			Mem[MEMORY_SIZE];	// 1kB memory
byte_t 			V[16];				// 16 8-bits registers
word_t 			I;					// Memory pointer
mstimer_t	delay_timer;
mstimer_t	sound_timer;

void Core_Init();
void Core_LoadRom( char* );
void Core_SkipInstr();
instruction_t Core_ReadOpcode();
void Core_ExecuteInstr( instruction_t instr );

#endif /* CORE_H_ */
