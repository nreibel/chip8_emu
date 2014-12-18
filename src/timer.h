#ifndef TIMER_H_
#define TIMER_H_

#include "types.h""

typedef unsigned long timer_60hz_t;

void Timer_Set( timer_60hz_t *, byte_t );
byte_t Timer_Get( timer_60hz_t );

#endif /* TIMER_H_ */
