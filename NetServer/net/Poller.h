//Author: Shaoxiong Xing
#pragma once
#include <vector>
#include <map>
#include "NetServer/base/Timestamp.h"
#include "NetServer/net/EventLoop.h"

class Channel;

class Poller : noncopyable
{
public:
    typedef std::vector<Channel *> ChannelList;
    Poller(EventLoop *loop);
    ~Poller();

    Timestamp poll(int timeoutMs, ChannelList *activeChannels);

    void updateChannel(Channel *channel);

    void removeChannel(Channel *channel);

    bool hasChannel(Channel *channel) const;

    static Poller *newDefaultPoller(EventLoop *loop);

    void assertInLoopThread() const
    {
        ownerLoop_->assertInLoopThread();
    }

private:
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;

    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel *> ChannelMap;

    EventLoop *ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
};