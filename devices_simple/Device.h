
#pragma once
#include "messageQueue/MessageBus.h"
#include <thread>
#include <atomic>
#include <chrono>
using namespace mq;
class Device {
public:
    explicit Device(MessageBus& bus);
    void start(std::chrono::milliseconds period, std::chrono::milliseconds requestWait);
    void stop();

private:
    void loop();

    MessageBus& bus_;
    std::thread th_;
    std::atomic<bool> running_{false};
    std::chrono::milliseconds period_{200};
    std::chrono::milliseconds requestWait_{10};
    mq::Snapshot snapshot_;

};
