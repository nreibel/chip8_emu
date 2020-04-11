#ifndef _KEYS_H_
#define _KEYS_H_

#include <SDL2/SDL.h>
#include "types.h"

// Key mapping :
// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F

typedef enum
{
    KEY_0 = 0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    NUMBER_OF_KEYS
} Chip8_Key;

#define MAP_KEY_1     SDL_SCANCODE_1
#define MAP_KEY_2     SDL_SCANCODE_2
#define MAP_KEY_3     SDL_SCANCODE_3
#define MAP_KEY_C     SDL_SCANCODE_4
#define MAP_KEY_4     SDL_SCANCODE_Q
#define MAP_KEY_5     SDL_SCANCODE_W
#define MAP_KEY_6     SDL_SCANCODE_E
#define MAP_KEY_D     SDL_SCANCODE_R
#define MAP_KEY_7     SDL_SCANCODE_A
#define MAP_KEY_8     SDL_SCANCODE_S
#define MAP_KEY_9     SDL_SCANCODE_D
#define MAP_KEY_E     SDL_SCANCODE_F
#define MAP_KEY_A     SDL_SCANCODE_Z
#define MAP_KEY_0     SDL_SCANCODE_X
#define MAP_KEY_B     SDL_SCANCODE_C
#define MAP_KEY_F     SDL_SCANCODE_V
#define MAP_KEY_RESET SDL_SCANCODE_RETURN
#define MAP_KEY_EXIT  SDL_SCANCODE_ESCAPE
#define MAP_KEY_SAVE  SDL_SCANCODE_F5
#define MAP_KEY_LOAD  SDL_SCANCODE_F9

void Keys_Init();
void Keys_HandleEvent(SDL_Event*);
bool_t Keys_IsPressed(Chip8_Key k);
Chip8_Key Keys_GetKey();

#endif /* _KEYS_H_ */
