#include <stdio.h>
#include <time.h>
#include <sys/time.h>

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

        unsigned short instr = Core_ReadOpcode();
        Core_ExecuteInstr( instr );

        while ( get_timestamp() - t_begin < 2 ) {}
    }
    
    return 0;
}

unsigned long get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (unsigned long) (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
