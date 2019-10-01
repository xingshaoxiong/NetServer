//Author: Shaoxiong Xing
#pragma once
#include "NetServer/base/noncopyable.h"
#include "NetServer/base/Types.h"

#include <functional>
#include <memory>
#include <vector>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    EventLoopThreadPool(EventLoop *baseloop, const string &nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { numThreads = numThreads; }
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    EventLoop *getNextLoop();
    EventLoop *getLoopForHash(size_t hashCode);

    std::vector<EventLoop *> getAllLoops();

    bool started() const
    {
        return started_;
    }
    const string &name() const
    {
        return name_;
    }
private:
    EventLoop *baseLoop_;
    string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};