#ifndef TIMER_H_
#define TIMER_H_

#include "types.h"

typedef unsigned long int mstimer_t;

void Timer_Set(mstimer_t *, unsigned int);
unsigned int Timer_Get(mstimer_t);

#endif /* TIMER_H_ */
