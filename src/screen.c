#include "screen.h"

SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;

void Screen_Init()
{
    window = SDL_CreateWindow(
        "Chip8 Emu",             // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        WINDOW_WIDTH,            // width, in pixels
        WINDOW_HEIGHT,           // height, in pixels
        SDL_WINDOW_OPENGL        // flags - see below
    );

    renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Screen_DrawScreen(screen_t screen)
{
    int xFactor = WINDOW_WIDTH / SCREEN_WIDTH;
    int yFactor = WINDOW_HEIGHT / SCREEN_HEIGHT;

    SDL_Rect rect = { 0, 0, xFactor - PIXEL_MARGIN, yFactor - PIXEL_MARGIN };

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 127, 127, SDL_ALPHA_OPAQUE);
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            if ( screen[x][y] != 0 )
            {
                rect.x = xFactor * x + PIXEL_MARGIN;
                rect.y = yFactor * y + PIXEL_MARGIN;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
