#pragma once

#include "messageQueue/MessageBus.h"
#include "messageQueue/example/Device.h"

#include <atomic>
#include <thread>
#include <string>

namespace mq::example {

/**
 * @brief Controller is created on the UI thread.
 * It owns MessageBus + Device and starts the device thread.
 *
 * UI thread responsibilities:
 *  - create Controller
 *  - call start()
 *  - call sendTaskA/sendTaskB
 *  - periodically call pumpUi() (e.g., Qt QTimer, or your UI event tick)
 *  - call stop() on shutdown
 */
class Controller {
public:
    Controller();
    ~Controller();

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    /**
     * @brief Start the device thread (idempotent).
     * @return true if started; false if already running.
     */
    bool start();

    /**
     * @brief Stop device thread and shutdown store (idempotent).
     * Safe to call from UI thread during shutdown.
     */
    void stop();

    /**
     * @brief Send TaskA request from UI thread.
     * @param payload Request payload.
     * @return request id (>0) on success; 0 if stopping.
     */
    std::uint64_t sendTaskA(std::string payload);

    /**
     * @brief Send TaskB request from UI thread.
     * @param payload Request payload.
     * @return request id (>0) on success; 0 if stopping.
     */
    std::uint64_t sendTaskB(std::string payload);

    /**
     * @brief UI thread pumps responses and runs callbacks.
     * Call this periodically (e.g., every 5-16ms).
     */
    void pumpUi();

private:
    mq::MessageBus store_;
    mq::example::Device device_;

    std::thread deviceThread_;
    std::atomic<bool> running_{false};
};

} // namespace mq::example
