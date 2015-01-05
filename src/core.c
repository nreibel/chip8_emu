#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "core.h"
#include "opcodes.h"
#include "screen.h"
#include "keys.h"
#include "stack.h"
#include "timer.h"

charset_t font = {
    { 0xF0, 0x90, 0x90, 0x90, 0xF0 }, // 0
    { 0x20, 0x60, 0x20, 0x20, 0x70 }, // 1
    { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }, // 2
    { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }, // 3
    { 0x90, 0x90, 0xF0, 0x10, 0x10 }, // 4
    { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }, // 5
    { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }, // 6
    { 0xF0, 0x10, 0x20, 0x40, 0x40 }, // 7
    { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }, // 8
    { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }, // 9
    { 0xF0, 0x90, 0xF0, 0x90, 0x90 }, // A
    { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }, // B
    { 0xF0, 0x80, 0x80, 0x80, 0xF0 }, // C
    { 0xE0, 0x90, 0x90, 0x90, 0xE0 }, // D
    { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }, // E
    { 0xF0, 0x80, 0xF0, 0x80, 0x80 }  // F
};

void Core_Init() {
    srand(time(NULL));

    // Reset memory
    memset(Mem, 0, MEMORY_SIZE * sizeof(byte_t));

    // Reset registers
    memset(V, 0, 16 * sizeof(byte_t));

    // Load font into memory
    memcpy(Mem + FONT_BASE_ADDR, font, sizeof(charset_t));

    // Set program counter to first instruction
    PC = PROGRAM_BASE_ADDR;

    // Set other registers
    I = 0;
    sound_timer = 0;
    delay_timer = 0;
}

int Core_LoadRom(char * path) {
    FILE * fp = fopen(path, "r");
    if ( fp == NULL ) {
        return -1;
    }

    // Get ROM size
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Load into memory
    fread(Mem + PROGRAM_BASE_ADDR, sizeof(byte_t), fsize, fp);

    fclose(fp);
    return 0;
}

instruction_t Core_ReadOpcode() {
    byte_t high = Mem[PC++];
    byte_t low  = Mem[PC++];
    return high << 8 | low;
}

void Core_SkipInstr() {
    PC += sizeof(instruction_t);
}

void Core_ExecuteInstr(instruction_t instr) {

    byte_t x        = MASK(instr, 0x0F00, 8);
    byte_t y        = MASK(instr, 0x00F0, 4);
    byte_t nibble   = MASK(instr, 0x000F, 0);
    byte_t byte     = MASK(instr, 0x00FF, 0);
    word_t addr     = MASK(instr, 0x0FFF, 0);

    if (OPCODE_MOV_VAL(instr)) {
        DBG_PRINT(("Set V[%u] to %u\n", x, byte));
        V[x] = byte;
    } else if (OPCODE_ADD_VAL(instr)) {
        DBG_PRINT(("Add %u to V[%u]\n", byte, x));
        V[x] += byte;
    } else if (OPCODE_MOV_REG(instr)) {
        DBG_PRINT(("Set V[%u] to V[%u]\n", x, y));
        V[x] = V[y];
    } else if (OPCODE_AND(instr)) {
        DBG_PRINT(("V[%u] &= V[%u]\n", x, y));
        V[x] &= V[y];
    } else if (OPCODE_XOR(instr)) {
        DBG_PRINT(("V[%u] ^= V[%u]\n", x, y));
        V[x] ^= V[y];
    } else if (OPCODE_ADD_REG(instr)) {
        DBG_PRINT(("V[%u] += V[%u] = ", x, y));
        V[15] = (V[x] + V[y] > 0xFF ? 1 : 0);
        V[x] += V[y];
    } else if (OPCODE_SUB_REG(instr)) {
        DBG_PRINT(("V[%u] -= V[%u]", x, y));
        V[15] = (V[x] < V[y] ? 0 : 1);
        V[x] -= V[y];
    } else if (OPCODE_LSR(instr)) {
        DBG_PRINT(("V[%u] >>= 1\n", x));
        V[15] = MASK(V[x], 0x1, 0);
        V[x] >>= 1;
    } else if (OPCODE_LSL(instr)) {
        DBG_PRINT(("V[%u] <<= 1\n", x));
        V[15] = MASK(V[x], 0x80, 7);
        V[x] <<= 1;
    } else if (OPCODE_SKIP_REG_EQ(instr)) {
        DBG_PRINT(("Skip if V[%u] == V[%u]\n", x, y));
        if (V[x] == V[y]) {
            Core_SkipInstr();
        }
    } else if (OPCODE_RAND(instr)) {
        DBG_PRINT(("Set V[%u] to random & %u\n", x, byte));
        V[x] = (byte_t) (rand() & byte);
    } else if (OPCODE_SKIP_EQ(instr)) {
        DBG_PRINT(("Skip if %u == %u\n", V[x], byte));
        if (V[x] == byte) {
            Core_SkipInstr();
        }
    } else if (OPCODE_SKIP_NEQ(instr)) {
        DBG_PRINT(("Skip if %u != %u\n", V[x], byte));
        if (V[x] != byte) {
            Core_SkipInstr();
        }
    } else if (OPCODE_MOV_I(instr)) {
        DBG_PRINT(("Set I to 0x%03X\n", addr));
        I = addr;
    } else if (OPCODE_DRAW(instr)) {
        DBG_PRINT(("Draw %u rows at (%u,%u)\n", nibble, V[x], V[y]));
        int dX, dY;
        int ptr = I;
        bool_t clear = false;
        for (dY = 0; dY < nibble; dY++) {
            unsigned char mask = Mem[ptr++];
            for (dX = 0; dX < 8; dX++) {
                if (mask & (0x80 >> dX)) {
                    clear |= Screen_SwitchPixel(V[x] + dX, V[y] + dY);
                }
            }
        }
        V[15] = (clear ? 1 : 0);
    } else if (OPCODE_RETURN(instr)) {
        DBG_PRINT(("Call return\n"));
        PC = Stack_Pop();
    } else if (OPCODE_CLS(instr)) {
        DBG_PRINT(("Clear screen\n"));
        Screen_Clear();
    } else if (OPCODE_BCD(instr)) {
        DBG_PRINT(("Store BCD value of %u at 0x%02x\n", V[x], I));

        int i;
        unsigned char buffer = V[x];
        for (i = 2; i >= 0; i--) {
            Mem[I + i] = buffer % 10;
            buffer /= 10;
            DBG_PRINT(("  0x%02x = %u\n", I+i, Mem[I+i]));
        }
    } else if (OPCODE_LDR(instr)) {
        DBG_PRINT(("Fill V[0] to V[%u] from 0x%02x\n", x, I));

        int i;
        for (i = 0; i <= x; i++) {
            V[i] = Mem[I + i];
            DBG_PRINT(("  V[%u] = Mem[%x] (0x%02x)\n", i, I+i, Mem[I+i]));
        }
    } else if (OPCODE_STR(instr)) {
        DBG_PRINT(("Store V[0] to V[%u] from 0x%02x\n", x, I));

        int i;
        for (i = 0; i <= x; i++) {
            Mem[I + i] = V[i];
            DBG_PRINT(("  Mem[%x] = V[%u] (0x%02x)\n", I+i, i, Mem[I+i]));
        }
    } else if (OPCODE_FONT(instr)) {
        DBG_PRINT(("Set I to char %x \n", V[x]));
        I = FONT_BASE_ADDR + (FONTS_HEIGHT * V[x]);
    } else if (OPCODE_SKIP_KEY_DOWN(instr)) {
        DBG_PRINT(("Check key %u pressed\n", V[x]));
        if (Keys_IsPressed(V[x])) {
            Core_SkipInstr();
        }
    } else if (OPCODE_SKIP_KEY_UP(instr)) {
        DBG_PRINT(("Check key %u released\n", V[x]));
        if (!Keys_IsPressed(V[x])) {
            Core_SkipInstr();
        }
    } else if (OPCODE_INPUT(instr)) {
        int reg = MASK(instr, 0x0F00, 8);
        DBG_PRINT(("Get key into V[%u]\n", reg));
        V[reg] = Keys_GetKey();
    } else if (OPCODE_SET_DELAY(instr)) {
        DBG_PRINT(("Set delay timer to %u ticks\n", V[x]));
        int ms = V[x] * 1000 / 60;
        Timer_Set(&delay_timer, ms);
    } else if (OPCODE_SET_SOUND(instr)) {
        DBG_PRINT(("Set sound timer to %u ticks\n", V[x]));
        int ms = V[x] * 1000 / 60;
        Timer_Set(&sound_timer, ms);
    } else if (OPCODE_GET_DELAY(instr)) {
        V[x] = Timer_Get(delay_timer);
        DBG_PRINT(("Delay timer %u stored in V[%u]\n", V[x], x));
    } else if (OPCODE_JUMP(instr)) {
        DBG_PRINT(("Jump to 0x%03X\n", addr));
        PC = addr;
    } else if (OPCODE_CALL(instr)) {
        DBG_PRINT(("Call subroutine at 0x%03X\n", addr));
        Stack_Push(PC);
        PC = addr;
    } else if (OPCODE_ADD_I(instr)) {
        DBG_PRINT(("Add V[%u] to I\n", x));
        I += V[x];
    } else {
        fprintf(stderr, "Unknown instruction %04X\n", instr);
        exit(0);
    }
}
