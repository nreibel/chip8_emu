#include "keys.h"
#include "core.h"
#include "screen.h"

bool_t keys[NUMBER_OF_KEYS];

void Keys_Init() {
    memset(keys, False, 16 * sizeof(bool_t));
}

bool_t Keys_IsPressed(Chip8_Key id) {
    return keys[id];
}

Chip8_Key Keys_GetKey() {

    SDL_Event event;

    while(True)
    {
        SDL_WaitEvent(&event);

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
                case MAP_KEY_0: return KEY_0;
                case MAP_KEY_1: return KEY_1;
                case MAP_KEY_2: return KEY_2;
                case MAP_KEY_3: return KEY_3;
                case MAP_KEY_4: return KEY_4;
                case MAP_KEY_5: return KEY_5;
                case MAP_KEY_6: return KEY_6;
                case MAP_KEY_7: return KEY_7;
                case MAP_KEY_8: return KEY_8;
                case MAP_KEY_9: return KEY_9;
                case MAP_KEY_A: return KEY_A;
                case MAP_KEY_B: return KEY_B;
                case MAP_KEY_C: return KEY_C;
                case MAP_KEY_D: return KEY_D;
                case MAP_KEY_E: return KEY_E;
                case MAP_KEY_F: return KEY_F;

                case MAP_KEY_SAVE:
                    Core_SaveState();
                    break;

                case MAP_KEY_LOAD:
                    Core_LoadState();
                    break;

                case MAP_KEY_EXIT:
                    Core_Exit();
                    break;

                case MAP_KEY_RESET:
                    Core_Reset();
                    break;

                default: break;
            }
        }
    }

    return NUMBER_OF_KEYS;
}

void Keys_HandleEvent(SDL_Event *event)
{
    bool_t keyState = event->type == SDL_KEYDOWN ? True : False;

    switch (event->key.keysym.scancode)
    {
        case MAP_KEY_0: keys[KEY_0] = keyState; return;
        case MAP_KEY_1: keys[KEY_1] = keyState; return;
        case MAP_KEY_2: keys[KEY_2] = keyState; return;
        case MAP_KEY_3: keys[KEY_3] = keyState; return;
        case MAP_KEY_4: keys[KEY_4] = keyState; return;
        case MAP_KEY_5: keys[KEY_5] = keyState; return;
        case MAP_KEY_6: keys[KEY_6] = keyState; return;
        case MAP_KEY_7: keys[KEY_7] = keyState; return;
        case MAP_KEY_8: keys[KEY_8] = keyState; return;
        case MAP_KEY_9: keys[KEY_9] = keyState; return;
        case MAP_KEY_A: keys[KEY_A] = keyState; return;
        case MAP_KEY_B: keys[KEY_B] = keyState; return;
        case MAP_KEY_C: keys[KEY_C] = keyState; return;
        case MAP_KEY_D: keys[KEY_D] = keyState; return;
        case MAP_KEY_E: keys[KEY_E] = keyState; return;
        case MAP_KEY_F: keys[KEY_F] = keyState; return;

        case MAP_KEY_SAVE:
        {
            if (event->type == SDL_KEYDOWN)
            {
                printf("Saving state\n");
                Core_SaveState();
            }
            break;
        }
        case MAP_KEY_LOAD:
        {
            if (event->type == SDL_KEYDOWN)
            {
                printf("Loading state\n");
                Core_LoadState();
            }
            break;
        }
        case MAP_KEY_EXIT:
        {
            if(event->type == SDL_KEYDOWN)
            {
                printf("Exit\n");
                Core_Exit();
            }
            break;
        }
        case MAP_KEY_RESET:
        {
            if(event->type == SDL_KEYDOWN)
            {
                printf("Reset\n");
                Core_Reset();
            }
            break;
        }
        default:
        {
            fprintf(stderr, "Unhandled key %d\n", event->key.keysym.scancode);
            break;
        }
    }
}
