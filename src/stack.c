#include "stack.h"

#include <string.h>

unsigned short  stack[STACK_SIZE];
unsigned short* SP;

void Stack_Init() {
	memset( stack, 0, STACK_SIZE * sizeof(unsigned short) );
	SP = stack;
}

void Stack_Push(unsigned short value) {
    *(SP++) = value;
}

unsigned short Stack_Pop() {
    return *(--SP);
}
