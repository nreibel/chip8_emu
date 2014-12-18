#ifndef _KEYS_H_
#define _KEYS_H_

#include <SDL/SDL.h>

#include "types.h"

#define KEY_A   0x18
#define KEY_Q   0x26
#define KEY_Z   0x19
#define KEY_S   0x27
#define KEY_ESC 0x09

#define MAP_KEY_01 KEY_Q
#define MAP_KEY_04 KEY_A
#define MAP_KEY_12 KEY_S
#define MAP_KEY_13 KEY_Z

void 	Keys_Init			();
void 	Keys_HandleEvent	( SDL_Event );
bool_t	Keys_IsPressed		( byte_t );

#endif /* _KEYS_H_ */
