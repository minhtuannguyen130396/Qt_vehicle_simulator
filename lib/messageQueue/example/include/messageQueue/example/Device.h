#pragma once

#include "messageQueue/MessageBus.h"
#include "messageQueue/Messages.h"

#include <chrono>

namespace mq::example {

/**
 * @brief Device owns heavy task logic and runs in the device thread.
 *
 * Usage:
 *  - Construct with a reference to MessageBus.
 *  - Call loop() on the device thread.
 *
 * The loop():
 *  - Waits for requests via MessageBus::deviceWaitRequest()
 *  - Executes heavy tasks in the device thread
 *  - Posts responses via MessageBus::devicePostResponse()
 */
class Device {
public:
    explicit Device(mq::MessageBus& store);

    /**
     * @brief Main device loop (run this in device thread).
     *
     * @param requestWait How long to wait per tick for a new request.
     * @param idleSleep If no request, optional sleep to reduce CPU.
     */
    void loop(std::chrono::milliseconds requestWait = std::chrono::milliseconds(2),
              std::chrono::milliseconds idleSleep  = std::chrono::milliseconds(1));

private:
    mq::Response handle(const mq::Request& req);

private:
    mq::MessageBus& store_;
};

} // namespace mq::example
