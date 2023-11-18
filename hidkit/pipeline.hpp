#ifndef PIPELINE_H
#define PIPELINE_H
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "packet.h"
#include "irouter.hpp"

class Pipeline
{
public:
    enum State {
        STATE_NONE = 0,
        STATE_PREPEND,
        STATE_RUN,
        STATE_REAY,
        STATE_EMPTY,
        STATE_TERMINATE
    };
    constexpr static int max_thread_num = 2;
protected:
    std::mutex mutex;
    std::condition_variable condit;
    std::deque<std::string> messageQueue;
    std::thread processThread[max_thread_num];
    std::shared_ptr<IRouter> router;
    int state;
protected:
    void process()
    {
        while (1) {
            std::string rawpacket;
            {
                std::unique_lock<std::mutex> locker(mutex);
                condit.wait(locker, [=](){
                    return state == STATE_REAY || state == STATE_TERMINATE;
                });
                if (state == STATE_TERMINATE) {
                    state = STATE_NONE;
                    break;
                }
                if (messageQueue.empty()) {
                    continue;
                }
                rawpacket.swap(messageQueue.front());
                messageQueue.pop_front();
                if (messageQueue.empty()) {
                    state = STATE_EMPTY;
                    condit.notify_all();
                }
                if (rawpacket.empty()) {
                    continue;
                }
            }

            /* handle message */
            if (router != nullptr) {
                router->invoke(rawpacket);
            }

        }
        return;
    }

public:
    Pipeline():router(nullptr),state(STATE_NONE){}
    ~Pipeline(){}
    void dispatch(const std::string &rawpacket)
    {
        std::unique_lock<std::mutex> locker(mutex);
        messageQueue.push_back(rawpacket);
        state = STATE_REAY;
        condit.notify_one();
        return;
    }

    void start()
    {
        if (state != STATE_NONE) {
            return;
        }
        for (std::size_t i = 0; i < max_thread_num; i++) {
            processThread[i] = std::thread(&Pipeline::process, this);
        }
        state = STATE_RUN;
        return;
    }

    void stop()
    {
        if (state == STATE_NONE) {
            return;
        }
        {
            std::unique_lock<std::mutex> locker(mutex);
            state = STATE_TERMINATE;
            condit.notify_all();
        }
        for (std::size_t i = 0; i < max_thread_num; i++) {
            processThread[i].join();
        }
        return;
    }

    void registerRouter(IRouter* pRouter)
    {
        router = std::shared_ptr<IRouter>(pRouter);
        return;
    }

};

#endif // PIPELINE_H
