#include <stdio.h>
#include <time.h>
#include "precisionTiming.h"

int main(void)
{
    struct timespec time;
    
    if(clock_gettime(CLOCK_MONOTONIC_RAW, &time))
    { 
        return -1;
    }

    precisionTime_t nano = getTimeNano();

    printf ("%lld\n", nano);
    printf("%lld.%.9ld\n", (precisionTime_t)time.tv_sec, time.tv_nsec);

    precisionDelayMicro(3000000);

    return 0;
}