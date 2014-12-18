#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL/SDL.h>

#include "types.h"

#define WINDOW_WIDTH    320
#define WINDOW_HEIGHT   160
#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT   32
#define COLOR_FRONT     0x008000
#define COLOR_BACK      0x000000

typedef unsigned char screen_t[SCREEN_WIDTH][SCREEN_HEIGHT];

void	Screen_Init			();
void	Screen_Refresh		();
bool_t	Screen_SwitchPixel	(int x, int y);
void	Screen_HandleEvent	( SDL_Event );

#endif
