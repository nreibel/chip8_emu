#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "opcodes.h"
#include "keys.h"
#include "timer.h"
#include "bits.h"

State state;
State save[SAVE_SLOTS];
int currentSlot = 0;

State *current = &state;

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

void Core_SaveState()
{
    printf("Saving state %d\n", currentSlot);
    memcpy( &save[currentSlot], &state, sizeof(State) );
    INC(currentSlot, SAVE_SLOTS);
}

void Core_LoadState()
{
    DEC(currentSlot, SAVE_SLOTS);
    printf("Loading state %d\n", currentSlot);
    memcpy( &state, &save[currentSlot], sizeof(State) );
    Screen_DrawScreen(current->screen);
}

void Core_Reset()
{
    // TODO
}

void Core_Exit()
{
    exit(0);
}

int Core_Init(char* rom_path)
{
    srand(time(NULL));

    FILE * fp = fopen(rom_path, "r");
    if ( fp == NULL )
    {
        printf("File %s not found\n", rom_path);
        return -1;
    }

    // Reset memory
    memset( current, 0, sizeof(State) );

    // Load font into memory
    memcpy(current->Mem + FONT_BASE_ADDR, font, sizeof(charset_t));

    // Get ROM size
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Load ROM into memory
    fread( current->Mem + PROGRAM_BASE_ADDR, sizeof(byte_t), fsize, fp );
    fclose(fp);

    // Init stack
    current->SP = &current->stack[0];

    // Set program counter to first instruction
    current->PC = PROGRAM_BASE_ADDR;

    return 0;
}

instruction_t Core_ReadOpcode()
{
    byte_t high = current->Mem[current->PC++];
    byte_t low  = current->Mem[current->PC++];
    return high << 8 | low;
}

void Core_SkipInstr() {
    current->PC += sizeof(instruction_t);
}

void Core_ExecuteInstr(instruction_t instr)
{
    bool_t refreshScreen = False;

    byte_t x        = MASK(instr, 0x0F00, 8);
    byte_t y        = MASK(instr, 0x00F0, 4);
    byte_t nibble   = MASK(instr, 0x000F, 0);
    byte_t byte     = MASK(instr, 0x00FF, 0);
    word_t addr     = MASK(instr, 0x0FFF, 0);

    if (OPCODE_CLS(instr)) // 00E0
    {
        DBG_PRINT(("CLS\n"));
        memset(current->screen, 0, sizeof(screen_t));
        refreshScreen = True;
    }
    else if (OPCODE_RETURN(instr)) // 00EE
    {
        DBG_PRINT(("RET\n"));
        current->PC = *(--current->SP);
    }
    else if (OPCODE_JUMP(instr)) // 1nnn
    {
        DBG_PRINT(("%-4s 0x%03X\n", "JP", addr));
        current->PC = addr;
    }
    else if (OPCODE_CALL(instr)) // 2nnn
    {
        DBG_PRINT(("%-4s 0x%03X\n", "CALL", addr));
        *(current->SP++) = current->PC;
        current->PC = addr;
    }
    else if (OPCODE_SKIP_EQ(instr)) // 3xkk
    {
        DBG_PRINT(("%-4s V[%X], %u\n", "SE", x, byte));
        if (current->V[x] == byte)
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_SKIP_NEQ(instr)) // 4xkk
    {
        DBG_PRINT(("%-4s V[%X], %u\n", "SNE", x, byte));
        if (current->V[x] != byte)
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_SKIP_EQ_REG(instr)) // 5xy0
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "SE", x, y));
        if (current->V[x] == current->V[y])
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_MOV_VAL(instr)) // 6xkk
    {
        DBG_PRINT(("%-4s V[%X], %u\n", "LD", x, byte));
        current->V[x] = byte;
    }
    else if (OPCODE_ADD_VAL(instr)) // 7xkk
    {
        DBG_PRINT(("%-4s V[%X], %u\n", "ADD", x, byte));
        current->V[x] += byte;
    }
    else if (OPCODE_MOV_REG(instr)) // 8xy0
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "LD", x, y));
        current->V[x] = current->V[y];
    }
    else if (OPCODE_OR(instr)) // 8xy1
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "OR", x, y));
        current->V[x] |= current->V[y];
    }
    else if (OPCODE_AND(instr)) // 8xy2
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "AND", x, y));
        current->V[x] &= current->V[y];
    }
    else if (OPCODE_XOR(instr)) // 8xy3
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "XOR", x, y));
        current->V[x] ^= current->V[y];
    }
    else if (OPCODE_ADD_REG(instr)) // 8xy4
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "ADD", x, y));
        word_t result = ((word_t) current->V[x]) + ((word_t) current->V[y]);
        current->V[0xF] = result > 0xFF ? 1 : 0;
        current->V[x] = (byte_t) result;
    }
    else if (OPCODE_SUB_REG(instr)) // 8xy5
    {
        DBG_PRINT(("%-4s V[%X], V[%X]", "SUB", x, y));
        current->V[0xF] = current->V[x] > current->V[y] ? 1 : 0;
        current->V[x] -= current->V[y];
    }
    else if (OPCODE_LSR(instr)) // 8xy6
    {
        DBG_PRINT(("%-4s V[%X]\n", "SHR", x));
        current->V[0xF] = BIT(current->V[x], 1);
        current->V[x] /= 2;
    }
    else if (OPCODE_SUBN(instr)) // 8xy7
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "SUBN", x, y));
        current->V[0xF] = current->V[y] > current->V[x] ? 1 : 0;
        current->V[x] = current->V[y] - current->V[x];
    }
    else if (OPCODE_LSL(instr)) // 8xyE
    {
        DBG_PRINT(("%-4s V[%X]\n", "SHL", x));
        current->V[0xF] = BIT(current->V[x], 7);
        current->V[x] *= 2;
    }
    else if (OPCODE_SKIP_REG_NEQ(instr)) // 9xy0
    {
        DBG_PRINT(("%-4s V[%X], V[%X]\n", "SNE", x, y));
        if (current->V[x] != current->V[y])
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_MOV_I(instr)) // Annn
    {
        DBG_PRINT(("%-4s I, 0x%03X\n", "LD", addr));
        current->I = addr;
    }
    else if (OPCODE_JUMP_REL(instr)) // Bnnn
    {
        DBG_PRINT(("%-4s V0, %u\n", "JP", addr));
        current->PC = current->V[0] + addr;
    }
    else if (OPCODE_RAND(instr)) // Cxkk
    {
        DBG_PRINT(("%-4s V[%X], %u\n", "RAND", x, byte));
        current->V[x] = (byte_t) (rand() & byte);
    }
    else if (OPCODE_DRAW(instr)) // Dxyn
    {
        DBG_PRINT(("%-4s V[%X], V[%X], %u\n", "DRW", x, y, nibble));

        current->V[0xF] = 0;

        for (int dy = 0; dy < nibble; dy++)
        {
            int py = (current->V[y] + dy) % SCREEN_HEIGHT;

            byte_t b = current->Mem[current->I + dy];

            for (int dx = 0; dx < 8; dx++)
            {
                int px = (current->V[x] + dx) % SCREEN_WIDTH;

                if ( BIT(b, 7-dx) )
                {
                    if( current->screen[px][py] ) current->V[0xF] = 1;
                    current->screen[px][py] = current->screen[px][py] ? 0 : 1;
                }
            }
        }

        refreshScreen = True;
    }
    else if (OPCODE_SKIP_KP(instr)) // Ex9E
    {
        DBG_PRINT(("%-4s V[%X]\n", "SKP", x));
        if (Keys_IsPressed(current->V[x]))
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_SKIP_NKP(instr)) // ExA1
    {
        DBG_PRINT(("%-4s V[%X]\n", "SKNP", x));
        if (!Keys_IsPressed(current->V[x]))
        {
            Core_SkipInstr();
        }
    }
    else if (OPCODE_GET_DELAY(instr)) // Fx07
    {
        DBG_PRINT(("%-4s V[%X], DT\n", "LD", x));
        current->V[x] = Timer_Get(current->delay_timer);
    }
    else if (OPCODE_INPUT(instr)) // Fx0A
    {
        DBG_PRINT(("%-4s V[%X], K\n", "LD", x));
        current->V[x] = Keys_GetKey();
    }
    else if (OPCODE_SET_DELAY(instr)) // Fx15
    {
        DBG_PRINT(("%-4s DT, V[%X]\n", "LD", x));
        int ms = current->V[x] * 1000 / 60;
        Timer_Set(&current->delay_timer, ms);
    }
    else if (OPCODE_SET_SOUND(instr)) // Fx18
    {
        DBG_PRINT(("%-4s ST, V[%X]\n", "LD", x));
        int ms = current->V[x] * 1000 / 60;
        Timer_Set(&current->sound_timer, ms);
    }
    else if (OPCODE_ADD_I(instr)) // Fx1E
    {
        DBG_PRINT(("%-4s I, V[%X]\n", "ADD", x));
        current->I = (current->I + current->V[x]) & 0xFFF;
    }
    else if (OPCODE_FONT(instr)) // Fx29
    {
        DBG_PRINT(("%-4s F, V[%X]\n", "LD", x));
        current->I = FONT_BASE_ADDR + (5 * current->V[x]);
    }
    else if (OPCODE_BCD(instr)) // Fx33
    {
        DBG_PRINT(("%-4s B, V[%X]\n", "LD", x));
        unsigned char buffer = current->V[x];
        for (int i = 2; i >= 0; i--)
        {
            current->Mem[current->I + i] = buffer % 10;
            buffer /= 10;
        }
    }
    else if (OPCODE_STR(instr)) // Fx55
    {
        DBG_PRINT(("%-4s [I], V[%X]\n", "LD",  x));
        for (int i = 0; i <= x; i++)
        {
            current->Mem[current->I + i] = current->V[i];
        }
    }
    else if (OPCODE_LDR(instr)) // Fx65
    {
        DBG_PRINT(("%-4s V[%X], [I]\n", "LD", x));
        for (int i = 0; i <= x; i++)
        {
            current->V[i] = current->Mem[current->I + i];
        }
    }
    else
    {
        fprintf(stderr, "Unknown instruction %04X\n", instr);
        exit(0);
    }

    if (refreshScreen)
    {
        Screen_DrawScreen(current->screen);
    }
}
