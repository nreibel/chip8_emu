#include "keys.h"
#include "main.h"

bool_t keys[16];

void Keys_Init() {
    memset(keys, false, 16 * sizeof(byte_t));
}

bool_t Keys_IsPressed(byte_t id) {
    return keys[id];
}

byte_t Keys_GetKey() {

    SDL_Event event;

    for (;;) {

        SDL_WaitEvent(&event);

        if (event.type != SDL_KEYDOWN) {
            continue;
        }

        switch (event.key.keysym.scancode) {
            case MAP_KEY_0: return 0x0;
            case MAP_KEY_1: return 0x1;
            case MAP_KEY_2: return 0x2;
            case MAP_KEY_3: return 0x3;
            case MAP_KEY_4: return 0x4;
            case MAP_KEY_5: return 0x5;
            case MAP_KEY_6: return 0x6;
            case MAP_KEY_7: return 0x7;
            case MAP_KEY_8: return 0x8;
            case MAP_KEY_9: return 0x9;
            case MAP_KEY_A: return 0xA;
            case MAP_KEY_B: return 0xB;
            case MAP_KEY_C: return 0xC;
            case MAP_KEY_D: return 0xD;
            case MAP_KEY_E: return 0xE;
            case MAP_KEY_F: return 0xF;
            case KEY_ESC:   exit(0);
        }
    }

    // Should not get here.
    return 0xFF;
}

void Keys_HandleEvent(SDL_Event event) {

    // Avoid events that are not key press
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
        return;
    }

    switch (event.key.keysym.scancode) {
        case MAP_KEY_0: keys[0x0] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_1: keys[0x1] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_2: keys[0x2] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_3: keys[0x3] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_4: keys[0x4] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_5: keys[0x5] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_6: keys[0x6] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_7: keys[0x7] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_8: keys[0x8] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_9: keys[0x9] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_A: keys[0xA] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_B: keys[0xB] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_C: keys[0xC] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_D: keys[0xD] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_E: keys[0xE] = (event.type == SDL_KEYDOWN ? true : false); return;
        case MAP_KEY_F: keys[0xF] = (event.type == SDL_KEYDOWN ? true : false); return;

        case KEY_ESC:   exit(0);
        case KEY_ENTER: Reset(); return;
        default: fprintf(stderr, "Unhandled key 0x%02x\n", event.key.keysym.scancode);
    }
}
