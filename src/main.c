#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"
#include "opcodes.h"
#include "sprites.h"
#include "screen.h"
#include "stack.h"
#include "keys.h"

unsigned short  PC = PROGRAM_BASE_ADDR;
unsigned char   Mem[MEMORY_SIZE];
unsigned char   V[16];
unsigned short  I;
unsigned long   sound_timer;
unsigned long   timer;

void init() {
    // Init random timer
    srand(time(NULL));

    // Reset memory
    memset(Mem, 0, MEMORY_SIZE);

    // Load font
    memcpy(Mem + FONT_BASE_ADDR, font, FONTS_HEIGHT*16);
}

void load (char* path) {
    FILE * f = fopen(path, "r");

    // Get ROM size
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Load into memory
    fread( Mem + PROGRAM_BASE_ADDR, fsize, 1, f );

    fclose(f);
}

int main(void) {

    init();

    Screen_Init();
    Keys_Init();
    Stack_Init();

    load("roms/PONG");

    SDL_Event event;

    for(;;) {

        unsigned long t_begin = get_timestamp();

    	// Handle events
        while( SDL_PollEvent( &event ) ) {
        	Screen_HandleEvent( event );
        	Keys_HandleEvent( event );
        }

        unsigned short instr = read_opcode();

        if ( OPCODE_MOV_VAL(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            int val = MASK(instr, 0x00FF, 0);
            DBG_PRINT(("Set V[%u] to %u\n", reg, val));
            V[reg] = val;
        }
        else if ( OPCODE_ADD_VAL(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            int val = MASK(instr, 0x00FF, 0);
            DBG_PRINT(("Add %u to V[%u]\n", val, reg));
            V[reg] += val;
        }
        else if ( OPCODE_MOV_REG(instr) ) {
            int x = MASK(instr, 0x0F00, 8);
            int y = MASK(instr, 0x00F0, 4);
            DBG_PRINT(("Set V[%u] to V[%u]\n", x, y));
            V[x] = V[y];
        }
        else if ( OPCODE_AND_REG(instr) ) {
            int x = MASK(instr, 0x0F00, 8);
            int y = MASK(instr, 0x00F0, 4);
            DBG_PRINT(("V[%u] &= V[%u]\n", x, y));
            V[x] &= V[y];
        }
        else if ( OPCODE_ADD_REG(instr) ) {
            int x = MASK(instr, 0x0F00, 8);
            int y = MASK(instr, 0x00F0, 4);
            DBG_PRINT(("V[%u] += V[%u]\n", x, y));

            V[15] = V[x] + V[y] > 0xFF;
			V[x] += V[y];
        }
        else if ( OPCODE_SUB_REG(instr) ) {
            int x = MASK(instr, 0x0F00, 8);
            int y = MASK(instr, 0x00F0, 4);
            DBG_PRINT(("V[%u] -= V[%u]\n", x, y));

            V[15] = V[x] > V[y];
			V[x] -= V[y];
        }
        else if ( OPCODE_RAND(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            int mask = MASK(instr, 0x00FF, 0);
            DBG_PRINT(("Set V[%u] to random < %u\n", reg, mask));
            V[reg] = (unsigned char) (rand() & mask);
        }
        else if ( OPCODE_SKIP_EQ(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            int val = MASK(instr, 0x00FF, 0);
            DBG_PRINT(("Skip if V[%u] (%u) == %u\n", reg, V[reg], val));

            if ( V[reg] == val ) {
            	skip_opcode();
            }
        }
        else if ( OPCODE_SKIP_NEQ(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            int val = MASK(instr, 0x00FF, 0);
            DBG_PRINT(("Skip if V[%u] (%u) != %u\n", reg, V[reg], val));

            if ( V[reg] != val ) {
            	skip_opcode();
            }
        }
        else if ( OPCODE_MOV_I(instr) ) {
            unsigned short val = MASK(instr, 0x0FFF, 0);
            DBG_PRINT(("Set I to 0x%03X\n", val));
            I = val;
        }
        else if ( OPCODE_DRAW(instr) ) {
            int x    = MASK(instr, 0x0F00, 8);
            int y    = MASK(instr, 0x00F0, 4);
            int height  = MASK(instr, 0x000F, 0);
            DBG_PRINT(("Draw %u rows at (%u,%u)\n", height, V[x], V[y]));

            int dX, dY;
            int addr = I;
            V[15] = 0;

            for ( dY = 0 ; dY < height ; dY++ ) {
                unsigned char mask = Mem[addr++];
                for ( dX = 0 ; dX < 8 ; dX++ ) {
                    if ( mask & (0x80 >> dX) ) {
                        V[15] |= Screen_SwitchPixel(V[x] + dX, V[y] + dY);
                    }
                }
            }

            Screen_Refresh();
        }
        else if ( OPCODE_RETURN(instr) ) {
            DBG_PRINT(("Call return\n"));
            PC = Stack_Pop();
        }
        else if ( OPCODE_BCD(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Store BCD value of %u at 0x%02x\n", V[reg], I));

            int i;
            unsigned char buffer = V[reg];
            for ( i = 2 ; i >= 0 ; i-- ) {
                Mem[I+i] = buffer % 10;
                buffer /= 10;
                DBG_PRINT(("  0x%02x = %u\n", I+i, Mem[I+i]));
            }
        }
        else if ( OPCODE_LDR(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Fill V[0] to V[%u] from 0x%02x\n", reg, I));

            int i;
            for ( i = 0 ; i <= reg ; i++ ) {
                V[i] = Mem[I+i];
                DBG_PRINT(("  V[%u] = Mem[%x] (0x%02x)\n", i, I+i, Mem[I+i]));
            }
        }
        else if ( OPCODE_FONT(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Set I to char %x \n", V[reg]));
            I = FONT_BASE_ADDR + (FONTS_HEIGHT * V[reg]);
        }
        else if ( OPCODE_SKIP_KEY_UP(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Check key %u released\n", V[reg]));
            
            if ( !Keys_IsPressed( V[reg] ) ) {
            	skip_opcode();
            }
        }
        else if ( OPCODE_SET_DELAY(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Set delay timer to %u ms\n", V[reg]));
            timer = get_timestamp() + V[reg];
        }
        else if ( OPCODE_SET_SOUND(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Set sound timer to V[%u]\n", reg));
            sound_timer = V[reg];
        }
        else if ( OPCODE_GET_DELAY(instr) ) {
            int reg = MASK(instr, 0x0F00, 8);
            DBG_PRINT(("Delay timer value %ld stored to V[%u]\n", timer, reg));

            unsigned long now = get_timestamp();
            if ( now > timer ) {
                V[reg] = 0;
            }
            else {
                V[reg] = (unsigned char) (timer - now);
            }
        }
        else if ( OPCODE_JUMP(instr) ) {
            int jmp = MASK(instr, 0x0FFF, 0);
            DBG_PRINT(("Jump to 0x%03X\n", jmp));
            PC = jmp;
        }
        else if ( OPCODE_CALL(instr) ) {
            unsigned short jmp = MASK(instr, 0x0FFF, 0);
            DBG_PRINT(("Call subroutine at 0x%03X\n", jmp));
            Stack_Push( PC );
            PC = jmp;
        }
        else {
            DBG_PRINT(("Unknown instruction %04X\n", instr));
            break;
        }

        while ( get_timestamp() - t_begin < 2 ) {}
    }
    
    return 0;
}

unsigned short read_opcode() {
    unsigned char high_byte = Mem[PC++];
    unsigned char low_byte  = Mem[PC++];
    return high_byte << 8 | low_byte;
}

void skip_opcode() {
	PC += 2;
}

unsigned short get_sound() {
    return sound_timer--;
}

unsigned long get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (unsigned long) (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
