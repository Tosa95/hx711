/*
*   Author: Tosatto Davide
*
*   Module for precision time measurements
*/

typedef long long int precisionTime_t;

precisionTime_t getTimeMicro ();
precisionTime_t getTimeNano ();
void precisionDelayMicro (precisionTime_t micro);