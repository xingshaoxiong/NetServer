//Author: Shaoxiong Xing
#include "NetServer/net/EventLoop.h"
#include "NetServer/base/MutexLock.h"

#include <assert.h>
#include <poll.h>

__thread EventLoop *t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop *EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      callingPendingFuctors_(false),
      threadId_(CurrentThread::tid())
{
    if (t_loopInThisThread)
    {
    }
    else
    {
        t_loopInThisThread == this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while (!quit_)
    {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        for (ChannelList::iterator it = activeChannels_.begin();
             it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
}

void EventLoop ::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}