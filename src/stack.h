#ifndef _STACK_H_
#define _STACK_H_

#include "types.h"

#define STACK_SIZE 16

void	Stack_Init	();
void	Stack_Push	( word_t );
word_t	Stack_Pop	();

#endif
