#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL/SDL.h>

#include "types.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   320
#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT   32
#define COLOR_FRONT     0x008000
#define COLOR_BACK      0x000000
#define PIXEL_MARGIN	1

typedef bool_t screen_t[SCREEN_WIDTH][SCREEN_HEIGHT];

void Screen_Init();
void Screen_Clear();
void Screen_Refresh();
bool_t Screen_SwitchPixel(int x, int y);
void Screen_HandleEvent(SDL_Event);

#endif
