//Author: Shaoxiong Xing
#pragma once
#include <functional>
#include <vector>
#include <atomic>

#include <boost/any.hpp>

#include "NetServer/base/MutexLock.h"
#include "NetServer/base/CurrentThread.h"
#include "NetServer/net/Channel.h"
#include "NetServer/base/Timestamp.h"
#include "NetServer/net/Poller.h"
#include "NetServer/net/TimerId.h"
#include "NetServer/net/Callbacks.h"

class Channel;
class Poller;
class TimerQueue;

class EventLoop : noncopyable
{
public:
    typedef std::function<void()> Functor;
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

    Timestamp pollReturnTime() const
    {
        return pollReturnTime_;
    }

    int64_t iteration() const
    {
        return iteration_;
    }

    void runInLoop(Functor cb);

    void queueInLoop(Functor cb);

    size_t queueSize() const;

    TimerId runAt(Timestamp time, TimerCallback cb);

    TimerId runAfter(double delay, TimerCallback cb);

    TimerId runEvery(double interval, TimerCallback cb);

    void cancel(TimerId timerId);

    static EventLoop *getEventLoopOfCurrentThread();

    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    bool isInLoopThread() const
    {
        return threadId_ == CurrentThread::tid();
    }

    bool eventHandling() const
    {
        return eventHandling_;
    }

    void setContext(const boost::any &context)
    {
        context_ = context;
    }

    const boost::any &getContext() const
    {
        return context_;
    }

    boost::any *getMutableContext()
    {
        return &context_;
    }

private:
    void abortNotInLoopThread();
    void handleRead();
    void doPendingFunctors();

    typedef std::vector<Channel *> ChannelList;

    bool looping_;
    std::atomic<bool> quit_;
    bool eventHandling_;
    bool callingPendingFuctors_;
    int64_t iteration_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    boost::any context_;
    ChannelList activeChannels_;
    Channel *currentActiveChannel_;
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
};