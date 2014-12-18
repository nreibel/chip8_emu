#include "screen.h"

screen_t screen;
SDL_Surface * sdl_surface;

void Screen_Init() {
    SDL_Init( SDL_INIT_VIDEO);
    sdl_surface = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Chip8 Emu", 0);
    memset(screen, false, sizeof(screen_t));
}

void Screen_Clear() {
    memset(screen, false, sizeof(screen_t));
}

void Screen_Refresh() {

    int xFactor = WINDOW_WIDTH / SCREEN_WIDTH;
    int yFactor = WINDOW_HEIGHT / SCREEN_HEIGHT;

    SDL_Rect rect = { 0, 0, xFactor - PIXEL_MARGIN, yFactor - PIXEL_MARGIN };

    int x, y;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            rect.x = xFactor * x + PIXEL_MARGIN;
            rect.y = yFactor * y + PIXEL_MARGIN;
            unsigned int color = (screen[x][y] == 0 ? COLOR_BACK : COLOR_FRONT);
            SDL_FillRect(sdl_surface, &rect, color);
        }
    }

    SDL_Flip(sdl_surface);
}

bool_t Screen_SwitchPixel(int x, int y) {

    y %= 32;
    x %= 64;

    screen[x][y] = !screen[x][y];
    return !screen[x][y];
}

void Screen_HandleEvent(SDL_Event event) {
    if (event.type == SDL_QUIT) {
        SDL_Quit();
    }
}
