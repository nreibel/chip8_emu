#ifndef _MAIN_H_
#define _MAIN_H_

#define MASK(val, mask, shift) (((val) & (mask)) >> (shift))

#ifdef DEBUG
    #define DBG_PRINT(str) printf str;
#else
    #define DBG_PRINT(str) {;}
#endif

#define MEMORY_SIZE         0x1000
#define PROGRAM_BASE_ADDR   0x200
#define FONT_BASE_ADDR      0x0

void			skip_opcode		();
unsigned short  read_opcode     ();
unsigned short  get_sound       ();
unsigned long   get_timestamp   ();
void            init            ();
void            load            (char* path);

#endif
