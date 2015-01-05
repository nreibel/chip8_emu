#include <stdio.h>

#include "main.h"
#include "opcodes.h"
#include "screen.h"
#include "stack.h"
#include "keys.h"
#include "core.h"
#include "timer.h"

#define REFRESH_RATE 32
#define CLOCK_SPEED  512

void Reset() {
    Core_Init();
    Keys_Init();
    Stack_Init();

    if ( Core_LoadRom(rom_path) < 0 ) {
        printf("File not found\n");
        exit(-2);
    }

    Screen_Init();
}

int main(int argc, char** argv) {

    if ( argc != 2 ) {
        printf("Missing path to rom\n");
        exit(-1);
    }
    else {
        rom_path = argv[1];
    }

    Reset();

    SDL_Event event;
    mstimer_t cycle_timer;

    for (;;) {

        Timer_Set(&cycle_timer, 1000 / REFRESH_RATE);

        // Handle events
        while (SDL_PollEvent(&event)) {
            Screen_HandleEvent(event);
            Keys_HandleEvent(event);
        }

        int cpt;
        for (cpt = 0; cpt < CLOCK_SPEED / REFRESH_RATE; cpt++) {
            instruction_t instr = Core_ReadOpcode();
            Core_ExecuteInstr(instr);
        }

        Screen_Refresh();

        while (Timer_Get(cycle_timer) > 0) {
        }
    }

    return 0;
}
