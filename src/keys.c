#include "keys.h"

unsigned char keys[16];

void Keys_Init() {
	memset( keys, 0, 16 * sizeof(unsigned char) );
}

unsigned char Keys_IsPressed( unsigned char id ) {
    return keys[id] != 0;
}

void Keys_HandleEvent( SDL_Event event ) {

	switch( event.key.keysym.scancode ) {
		case MAP_KEY_01:
			keys[1] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
			break;
		case MAP_KEY_04:
			keys[4] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
			break;
		case MAP_KEY_12:
			keys[12] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
			break;
		case MAP_KEY_13:
			keys[13] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
			break;
		case KEY_ESC:
			exit(0);
			break;
		default:
			printf("Unhandled key 0x%02x\n", event.key.keysym.scancode);
			break;
	}
}
