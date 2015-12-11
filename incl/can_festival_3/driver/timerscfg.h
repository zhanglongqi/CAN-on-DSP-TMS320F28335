#ifndef __TIMERSCFG_H__
#define __TIMERSCFG_H__

//#include <pthread.h>

// Time unit : us
// Time resolution : 64bit (~584942 years)
#define TIMEVAL unsigned long long
#define TIMEVAL_MAX ~(TIMEVAL)0
#define MS_TO_TIMEVAL(ms) ms*1000L
#define US_TO_TIMEVAL(us) us

//#define TASK_HANDLE pthread_t
#define TASK_HANDLE void*

#endif
