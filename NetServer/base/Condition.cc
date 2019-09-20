//Author: Shaoxiong Xing
#include "NetServer/base/Condition.h"
#include <errno.h>

bool Condition::waitForSeconds(double seconds)
{
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    const __int64_t kNanoSecondsPerSecond = 1000000000;
    __int64_t nanoseconds = static_cast<__int64_t>(seconds * kNanoSecondsPerSecond);
    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.get(), &abstime);
}