#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "core.h"
#include "opcodes.h"
#include "screen.h"
#include "keys.h"
#include "stack.h"

charset_t font = {
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
};

void Core_Init() {
	srand(time(NULL));

	// Reset memory
	memset( Mem, 0, MEMORY_SIZE*sizeof(byte_t) );

	// Reset registers
	memset( V, 0, 16*sizeof(byte_t) );

	// Load font into memory
	memcpy( Mem + FONT_BASE_ADDR, font, sizeof(charset_t) );

	// Set program counter to first instruction
	PC = PROGRAM_BASE_ADDR;

	// Set other registers
	I = 0;
	sound_timer = 0;
	delay_timer = 0;
}

void Core_LoadRom( char * path ) {
	FILE * fp = fopen(path, "r");

	// Get ROM size
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Load into memory
	fread( Mem + PROGRAM_BASE_ADDR, sizeof(byte_t), fsize, fp );

	fclose(fp);
}

instruction_t Core_ReadOpcode() {
	byte_t high = Mem[PC++];
	byte_t low  = Mem[PC++];
	return high << 8 | low;
}

void Core_SkipInstr() {
	PC += sizeof(instruction_t);
}

void Core_ExecuteInstr( instruction_t instr ) {

    if ( OPCODE_MOV_VAL(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        byte_t val = MASK(instr, 0x00FF, 0);
        DBG_PRINT(("Set V[%u] to %u\n", reg, val));
        V[reg] = val;
    }
    else if ( OPCODE_ADD_VAL(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        byte_t val = MASK(instr, 0x00FF, 0);
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
    else if ( OPCODE_XOR_REG(instr) ) {
        int x = MASK(instr, 0x0F00, 8);
        int y = MASK(instr, 0x00F0, 4);
        DBG_PRINT(("V[%u] ^= V[%u]\n", x, y));
        V[x] ^= V[y];
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
    else if ( OPCODE_LSL(instr) ) {
        int x = MASK(instr, 0x0F00, 8);
        int y = MASK(instr, 0x00F0, 4);
        DBG_PRINT(("V[%u] <<= 1\n", x));
        V[15] = V[x] >> 7;
        V[x] <<= 1;
    }
    else if ( OPCODE_SKIP_REG_EQ(instr) ) {
        int x = MASK(instr, 0x0F00, 8);
        int y = MASK(instr, 0x00F0, 4);
        DBG_PRINT(("Skip if V[%u] == V[%u]\n", x, y));

        if ( V[x] == V[y] ) {
        	Core_SkipInstr();
        }
    }
    else if ( OPCODE_RAND(instr) ) {
        int reg  = MASK(instr, 0x0F00, 8);
        byte_t mask = MASK(instr, 0x00FF, 0);
        DBG_PRINT(("Set V[%u] to random & %u\n", reg, mask));
        V[reg] = (byte_t) (rand() & mask);
    }
    else if ( OPCODE_SKIP_EQ(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        byte_t val = MASK(instr, 0x00FF, 0);
        DBG_PRINT(("Skip if %u == %u\n", V[reg], val));
        if ( V[reg] == val ) {
        	Core_SkipInstr();
        }
    }
    else if ( OPCODE_SKIP_NEQ(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        byte_t val = MASK(instr, 0x00FF, 0);
        DBG_PRINT(("Skip if %u != %u\n", V[reg], val));
        if ( V[reg] != val ) {
        	Core_SkipInstr();
        }
    }
    else if ( OPCODE_MOV_I(instr) ) {
        word_t val = MASK(instr, 0x0FFF, 0);
        DBG_PRINT(("Set I to 0x%03X\n", val));
        I = val;
    }
    else if ( OPCODE_DRAW(instr) ) {
        int x = MASK(instr, 0x0F00, 8);
        int y = MASK(instr, 0x00F0, 4);
        int h = MASK(instr, 0x000F, 0);
        DBG_PRINT(("Draw %u rows at (%u,%u)\n", h, V[x], V[y]));

        int dX, dY;
        int addr = I;
        bool_t clear = false;

        for ( dY = 0 ; dY < h ; dY++ ) {
            unsigned char mask = Mem[addr++];
            for ( dX = 0 ; dX < 8 ; dX++ ) {
                if ( mask & (0x80 >> dX) ) {
                    clear |= Screen_SwitchPixel(V[x] + dX, V[y] + dY);
                }
            }
        }

        V[15] = (clear ? 1 : 0);

        Screen_Refresh();
    }
    else if ( OPCODE_RETURN(instr) ) {
        DBG_PRINT(("Call return\n"));
        PC = Stack_Pop();
    }
    else if ( OPCODE_CLS(instr) ) {
        DBG_PRINT(("Call return\n"));
        Screen_Clear();
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
    else if ( OPCODE_STR(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Store V[0] to V[%u] from 0x%02x\n", reg, I));

        int i;
        for ( i = 0 ; i <= reg ; i++ ) {
            Mem[I+i] = V[i];
            DBG_PRINT(("  Mem[%x] = V[%u] (0x%02x)\n", I+i, i, Mem[I+i]));
        }
    }
    else if ( OPCODE_FONT(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Set I to char %x \n", V[reg]));
        I = FONT_BASE_ADDR + (FONTS_HEIGHT * V[reg]);
    }
    else if ( OPCODE_SKIP_KEY_DOWN(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Check key %u pressed\n", V[reg]));

        if ( Keys_IsPressed( V[reg] ) ) {
        	Core_SkipInstr();
        }
    }
    else if ( OPCODE_SKIP_KEY_UP(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Check key %u released\n", V[reg]));

        if ( !Keys_IsPressed( V[reg] ) ) {
        	Core_SkipInstr();
        }
    }
    else if ( OPCODE_INPUT(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Get key into V[%u]\n", reg));
        V[reg] = Keys_GetKey();
    }
    else if ( OPCODE_SET_DELAY(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Set delay timer to %u ms\n", V[reg]));
        delay_timer = get_timestamp() + V[reg];
    }
    else if ( OPCODE_SET_SOUND(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Set sound timer to V[%u]\n", reg));
        sound_timer = V[reg];
    }
    else if ( OPCODE_GET_DELAY(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Delay timer value %ld stored to V[%u]\n", delay_timer, reg));

        unsigned long now = get_timestamp();
        if ( now > delay_timer ) {
            V[reg] = 0;
        }
        else {
            V[reg] = (byte_t) (delay_timer - now);
        }
    }
    else if ( OPCODE_JUMP(instr) ) {
    	word_t jmp = MASK(instr, 0x0FFF, 0);
        DBG_PRINT(("Jump to 0x%03X\n", jmp));
        PC = jmp;
    }
    else if ( OPCODE_CALL(instr) ) {
        word_t jmp = MASK(instr, 0x0FFF, 0);
        DBG_PRINT(("Call subroutine at 0x%03X\n", jmp));
        Stack_Push( PC );
        PC = jmp;
    }
    else if ( OPCODE_ADD_I(instr) ) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Add V[%u] to I\n", delay_timer, reg));
        I += V[reg];
    }
    else {
        DBG_PRINT(("Unknown instruction %04X\n", instr));
        exit(0);
    }
}
