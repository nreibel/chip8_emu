#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL2/SDL.h>

#include "types.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   320
#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT   32
#define PIXEL_MARGIN	1


typedef byte_t screen_t[SCREEN_WIDTH][SCREEN_HEIGHT];

void Screen_Init();
void Screen_DrawScreen(screen_t screen);

#endif
