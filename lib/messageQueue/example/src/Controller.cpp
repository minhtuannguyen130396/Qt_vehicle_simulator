#include "messageQueue/example/Controller.h"

#include <iostream>

namespace mq::example {

Controller::Controller()
    : store_()
    , device_(store_) {}

Controller::~Controller() {
    stop();
}

bool Controller::start() {
    if (running_.exchange(true)) return false;

    deviceThread_ = std::thread([this] {
        // Device thread entry
        device_.loop();
    });

    return true;
}

void Controller::stop() {
    if (!running_.exchange(false)) return;

    store_.requestStop();

    if (deviceThread_.joinable())
        deviceThread_.join();

    store_.shutdown();
}

std::uint64_t Controller::sendTaskA(std::string payload) {
    return store_.enqueueRequest(mq::TaskType::TaskA, std::move(payload),
        [](const mq::Response& r) {
            // UI callback: safe for Qt UI updates
            std::cout << "[UI Callback] id=" << r.id
                      << " ok=" << r.ok
                      << " result=" << r.result << "\n";
        });
}

std::uint64_t Controller::sendTaskB(std::string payload) {
    return store_.enqueueRequest(mq::TaskType::TaskB, std::move(payload),
        [](const mq::Response& r) {
            std::cout << "[UI Callback] id=" << r.id
                      << " ok=" << r.ok
                      << " result=" << r.result << "\n";
        });
}

void Controller::pumpUi() {
    store_.uiPumpResponses(32);
}

} // namespace mq::example
