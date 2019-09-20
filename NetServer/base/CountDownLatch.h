//Author: Shaoxiong Xing
#pragma once
#include "NetServer/base/MutexLock.h"
#include "NetServer/base/Condition.h"

class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();
    
    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};