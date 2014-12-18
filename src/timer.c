#include <stdlib.h>
#include <sys/time.h>

#include "timer.h"

timer_60hz_t get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (timer_60hz_t) (tv.tv_usec / 1000);
}

void Timer_Set( timer_60hz_t *timer, byte_t value ) {
	*timer = get_timestamp() + value;
}


byte_t Timer_Get( timer_60hz_t timer ) {
    timer_60hz_t now = get_timestamp();
    if ( now > timer ) {
        return 0;
    }
    else {
        return (byte_t) (timer - now);
    }
}
