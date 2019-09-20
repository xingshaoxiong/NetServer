//Author: Shaoxiong Xing
#pragma once
#include <functional>
#include <vector>

#include "NetServer/base/MutexLock.h"
#include "NetServer/base/CurrentThread.h"
#include "NetServer/net/Channel.h"
#include "NetServer/base/Timestamp.h"
#include "NetServer/net/Poller.h"

#include <vector>
#include <functional>

class EventLoop : noncopyable
{
public:
    EventLoop();
    ~EventLoop();

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return threadId_ == CurrentThread::tid();
    }

    void loop();

    void quit();

    static EventLoop *getEventLoopOfCurrentThread();

    void updateChannel(Channel *channel);

private:
    void abortNotInLoopThread();
    void handleRead();
    void doPendingFunctors();

    typedef std::vector<Channel *> ChannelList;

    bool looping_;
    bool quit_;
    bool callingPendingFuctors_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    // std::unique_ptr<TimerQueue> timerQueue_;
    ChannelList activeChannels_;

};