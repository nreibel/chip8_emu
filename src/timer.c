#include <stdlib.h>
#include <sys/time.h>

#include "timer.h"

mstimer_t get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (mstimer_t) ((tv.tv_sec * 1000) + (tv.tv_usec / 1000) + 0.5);
}

void Timer_Set(mstimer_t *timer, unsigned int value)
{
    *timer = get_timestamp() + value;
}

unsigned int Timer_Get(mstimer_t timer) {
    mstimer_t now = get_timestamp();

    if (now > timer)
    {
        return 0;
    }
    else
    {
        mstimer_t diff = timer - now;

        // Never return zero unless the timer actually expired
        // Always set the last bit to make sure of that
        return (unsigned int) (diff | 1);
    }
}
