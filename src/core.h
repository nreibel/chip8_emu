#ifndef CORE_H_
#define CORE_H_

#define MEMORY_SIZE         0x1000
#define PROGRAM_BASE_ADDR   0x200
#define FONT_BASE_ADDR      0x0
#define FONTS_HEIGHT 5

typedef unsigned char charset_t[16][5];

unsigned short  PC;
unsigned char   Mem[MEMORY_SIZE];
unsigned char   V[16];
unsigned short  I;
unsigned long   sound_timer;
unsigned long   delay_timer;

void Core_Init();
void Core_LoadRom( char* );
void Core_SkipInstr();
unsigned short Core_ReadOpcode();
void Core_ExecuteInstr( unsigned short instr );

#endif /* CORE_H_ */
