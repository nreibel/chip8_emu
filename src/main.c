#include <stdio.h>

#include "main.h"
#include "opcodes.h"
#include "screen.h"
#include "stack.h"
#include "keys.h"
#include "core.h"

int main(void) {

    Core_Init();
    Screen_Init();
    Keys_Init();
    Stack_Init();

    Core_LoadRom("roms/PONG");

    SDL_Event event;

    for(;;) {

        unsigned long t_begin = get_timestamp();

    	// Handle events
        while( SDL_PollEvent( &event ) ) {
        	Screen_HandleEvent( event );
        	Keys_HandleEvent( event );
        }

        instruction_t instr = Core_ReadOpcode();
        Core_ExecuteInstr( instr );

        while ( get_timestamp() - t_begin < 2 ) {}
    }
    
    return 0;
}
