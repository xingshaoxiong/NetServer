//Author: Shaoxiong Xing
#pragma once

#include "NetServer/base/Condition.h"
#include "NetServer/base/MutexLock.h"
#include "NetServer/base/Thread.h"
#include "NetServer/base/Types.h"

class EventLoop;

class EventLoopThread : noncopyable
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const string &name = string());
    
    ~EventLoopThread();
    EventLoop *startLoop();

private:
    void threadFunc();

    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    ThreadInitCallback callback_;
};