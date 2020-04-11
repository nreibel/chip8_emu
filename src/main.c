#include <stdio.h>
#include <SDL2/SDL.h>

#include "core.h"
#include "keys.h"
#include "screen.h"

int main(int argc, char** argv)
{
    SDL_Event event;
    SDL_Init(SDL_INIT_EVERYTHING);

    if ( argc != 2 )
    {
        printf("Missing path to rom\n");
        exit(-1);
    }
    else if ( Core_Init( argv[1] ) != 0 )
    {
        printf("Failed to init core\n");
        exit(-2);
    }

    Keys_Init();
    Screen_Init();

    while(1)
    {
        uint32_t timeStart = SDL_GetTicks();

        // Handle events
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    Core_Exit();
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    Keys_HandleEvent(&event);
                    break;

                default:
                    // Do nothing
                    break;
            }
        }

        instruction_t instr = Core_ReadOpcode();
        Core_ExecuteInstr(instr);

        while (SDL_GetTicks() - timeStart < 1000/CLOCK_SPEED) SDL_Delay(1);
    }

    return 0;
}
