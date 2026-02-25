#include "messageQueue/example/Device.h"

#include <thread>

namespace mq::example {

Device::Device(mq::MessageBus& store) : store_(store) {}

void Device::loop(std::chrono::milliseconds requestWait, std::chrono::milliseconds idleSleep) {
    while (!store_.isStopping()) {
        mq::Request req;
        const bool got = store_.deviceWaitRequest(req, requestWait);

        // Here you can run periodic hardware work even if no request:
        // doHardwareTick();

        if (!got) {
            if (idleSleep.count() > 0) std::this_thread::sleep_for(idleSleep);
            continue;
        }

        mq::Response resp = handle(req);
        store_.devicePostResponse(std::move(resp));
    }

    // device cleanup if needed
}

mq::Response Device::handle(const mq::Request& req) {
    using namespace std::chrono_literals;

    // Simulate heavy work / hardware access
    std::this_thread::sleep_for(30ms);

    mq::Response r;
    r.id = req.id;
    r.ok = true;

    switch (req.type) {
        case mq::TaskType::TaskA:
            r.result = "TaskA processed: " + req.payload;
            break;
        case mq::TaskType::TaskB:
            r.result = "TaskB processed: " + req.payload;
            break;
        default:
            r.ok = false;
            r.result = "Unknown task";
            break;
    }
    return r;
}

} // namespace mq::example
