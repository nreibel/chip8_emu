#include "stack.h"

#include <string.h>

word_t   stack[STACK_SIZE];
word_t * SP;

void Stack_Init() {
	memset( stack, 0, STACK_SIZE * sizeof(word_t) );
	SP = stack;
}

void Stack_Push(word_t value) {
    *(SP++) = value;
}

word_t Stack_Pop() {
    return *(--SP);
}
