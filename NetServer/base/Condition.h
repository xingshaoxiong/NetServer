//Author: Shaoxiong Xing
#pragma once
#include "NetServer/base/noncopyable.h"
#include "NetServer/base/MutexLock.h"
#include <pthread.h>

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock &mutex)
        : mutex_(mutex)
    {
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&pcond_);
    }

    void wait()
    {
        pthread_cond_wait(&pcond_, mutex_.get);
    }

    //return true if time out, false else.
    bool waitForSeconds(double seconds);

    void notify()
    {
        pthread_cond_signal(&pcond_);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&pcond_);
    }

private:
    MutexLock &mutex_;
    pthread_cond_t pcond_;
};