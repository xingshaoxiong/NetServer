//Author: Shaoxiong Xing
#pragma once
#include <set>
#include <vector>
#include "NetServer/base/noncopyable.h"
#include "NetServer/base/Timestamp.h"
#include "NetServer/base/MutexLock.h"
#include "NetServer/net/Channel.h"
#include "NetServer/net/Callbacks.h"

class EventLoop;
class Timer;
class TimerId;
class TimerQueue : noncopyable
{
public:
    explicit TimerQueue(EventLoop *loop);
    ~TimerQueue();

    TimerId addTimer(TimerCallback cb, Timestamp when, double interval);

    void cancel(TimerId timerId);

private:
    typedef std::pair<Timestamp, Timer *> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer *, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer *timer);
    void cancelInLoop(TimerId timerId);

    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry> &expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop *loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;

    ActiveTimerSet activeTimers_;
    bool callingExpiredTimers_;
    ActiveTimerSet cancelingTimers_;
};