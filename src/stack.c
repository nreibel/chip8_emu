#include "stack.h"

#include <string.h>

unsigned short  buffer[STACK_SIZE];
unsigned short* stack_ptr;

void Stack_Init() {
	memset( buffer, 0, STACK_SIZE * sizeof(unsigned short) );
	stack_ptr = buffer;
}

void Stack_Push(unsigned short value) {
    *(stack_ptr++) = value;
}

unsigned short Stack_Pop() {
    return *(--stack_ptr);
}
