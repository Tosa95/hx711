/*
*   Author: Tosatto Davide
*
*   Module for precision time measurements
*/
#include <time.h>
#include "precisionTiming.h"

#define NANO_TO_MICRO(n) (n/1000L)
#define SEC_TO_NANO(s) (s*1000000000L)


static precisionTime_t timespecToMicro ();
static precisionTime_t timespecToNano (struct timespec *ts);

precisionTime_t getTimeMicro ()
{
    return NANO_TO_MICRO(getTimeNano ());
}

precisionTime_t getTimeNano ()
{
    struct timespec time;
    
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &time))
    { 
        return -1;
    }

    return timespecToNano(&time);
}

static precisionTime_t timespecToNano (struct timespec *ts)
{
    return SEC_TO_NANO((precisionTime_t)(ts->tv_sec)) + (precisionTime_t)(ts->tv_nsec);
}

static precisionTime_t timespecToMicro (struct timespec *ts)
{
    return NANO_TO_MICRO(timespecToNano(ts));
}

void precisionDelayMicro (precisionTime_t micro)
{
    precisionTime_t end = getTimeMicro() + micro;

    while (getTimeMicro()<end);
}