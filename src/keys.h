#ifndef _KEYS_H_
#define _KEYS_H_

#include <SDL/SDL.h>

#include "types.h"

#define KEY_1   	0x0A
#define KEY_2   	0x0B
#define KEY_3   	0x0C
#define KEY_4   	0x0D
#define KEY_A   	0x18
#define KEY_Z   	0x19
#define KEY_E   	0x1A
#define KEY_R   	0x1B
#define KEY_Q   	0x26
#define KEY_S   	0x27
#define KEY_D   	0x28
#define KEY_F   	0x29
#define KEY_W   	0x34
#define KEY_X   	0x35
#define KEY_C   	0x36
#define KEY_V   	0x37
#define KEY_ESC 	0x09
#define KEY_ENTER	0x24

#define MAP_KEY_1 KEY_1
#define MAP_KEY_2 KEY_2
#define MAP_KEY_3 KEY_3
#define MAP_KEY_4 KEY_A
#define MAP_KEY_5 KEY_Z
#define MAP_KEY_6 KEY_E
#define MAP_KEY_7 KEY_Q
#define MAP_KEY_8 KEY_S
#define MAP_KEY_9 KEY_D
#define MAP_KEY_A KEY_W
#define MAP_KEY_0 KEY_X
#define MAP_KEY_B KEY_C
#define MAP_KEY_C KEY_4
#define MAP_KEY_D KEY_R
#define MAP_KEY_E KEY_F
#define MAP_KEY_F KEY_V

void Keys_Init();
void Keys_HandleEvent(SDL_Event);
bool_t Keys_IsPressed(byte_t);
byte_t Keys_GetKey();

#endif /* _KEYS_H_ */
