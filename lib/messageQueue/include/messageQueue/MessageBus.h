#pragma once

#include "messageQueue/Messages.h"
#include "messageQueue/ThreadSafeQueue.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <chrono>

namespace mq {

/**
 * @brief MessageBus provides bidirectional messaging + callback routing.
 *
 * Ownership/threads:
 *  - Producers (UI/network/etc.) call enqueueRequest() from any thread.
 *  - Device thread calls deviceWaitRequest() to fetch work, then devicePostResponse().
 *  - UI thread calls uiPumpResponses() to execute callbacks on UI thread.
 *
 * IMPORTANT:
 *  - MessageBus does NOT run device loops; Device owns while(1).
 *  - Callbacks are invoked ONLY inside uiPumpResponses().
 */
class MessageBus {
public:
    using Callback = std::function<void(const Response&)>;

    MessageBus() = default;
    MessageBus(const MessageBus&) = delete;
    MessageBus& operator=(const MessageBus&) = delete;

    /**
     * @brief Enqueue a request and register a callback.
     *
     * @param type Task type (TaskA/TaskB/Stop). Stop is normally sent via requestStop().
     * @param payload Input payload for the task.
     * @param cb Callback invoked on UI thread when response arrives.
     *
     * @return Request id (>0) on success; 0 if store is stopping.
     */
    // std::uint64_t enqueueRequest(TaskType type, std::string payload, Callback cb);
    /**
     * @brief Enqueue a request and register a callback.
     *
     * @param type Task type (TaskA/TaskB/Stop). Stop is normally sent via requestStop().
     * @param Snapshot Input snapshot for the task.
     * @param cb Callback invoked on UI thread when response arrives.
     *
     * @return Request id (>0) on success; 0 if store is stopping.
     */
    std::uint64_t enqueueRequest(TaskType type, Snapshot snapshot, Callback cb);

    /**
     * @brief Request a graceful stop (in-band stop message).
     * Device thread will observe stop and exit its loop.
     */
    void requestStop();

    /**
     * @brief Device thread waits briefly for a request.
     *
     * @param out Filled with a request if returned true.
     * @param waitFor Max time to wait for a request (prevents busy-spin).
     *
     * @return true if a request is available (and not Stop).
     *         false if timeout/no request OR Stop was requested OR store is stopping.
     */
    bool deviceWaitRequest(Request& out, std::chrono::milliseconds waitFor);

    /**
     * @brief Device thread posts a response back to UI.
     *
     * @param r Response to push into response queue.
     */
    void devicePostResponse(Response r);

    /**
     * @brief UI thread drains responses and invokes callbacks.
     *
     * @param maxBatch Max number of responses to process per call.
     *                Use a bounded batch to keep UI responsive.
     */
    void uiPumpResponses(std::size_t maxBatch = 16);

    /**
     * @brief Query stop state (thread-safe).
     * @return true if stop requested / store stopping.
     */
    bool isStopping() const;

    /**
     * @brief Shutdown store: closes queues and clears callbacks.
     * Safe to call multiple times.
     */
    void shutdown();

private:
    ThreadSafeQueue<Request> requestQ_;
    ThreadSafeQueue<Response> responseQ_;

    std::atomic<bool> stopping_{false};
    std::atomic<bool> shutdownOnce_{false};
    std::atomic<std::uint64_t> nextId_{1};

    mutable std::mutex cbMutex_;
    std::unordered_map<std::uint64_t, Callback> callbacks_;
};

} // namespace mq
