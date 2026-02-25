#include "messageQueue/MessageBus.h"

namespace mq {

// std::uint64_t MessageBus::enqueueRequest(TaskType type, std::string payload, Callback cb) {
//     if (stopping_.load(std::memory_order_acquire)) return 0;

//     const auto id = nextId_.fetch_add(1, std::memory_order_relaxed);

//     {
//         std::lock_guard<std::mutex> lk(cbMutex_);
//         callbacks_[id] = std::move(cb);
//     }

//     requestQ_.push(Request{type, id, std::move(payload)});
//     return id;
// }


std::uint64_t MessageBus::enqueueRequest(TaskType type, Snapshot snapshot, Callback cb) {
    if (stopping_.load(std::memory_order_acquire)) return 0;

    const auto id = nextId_.fetch_add(1, std::memory_order_relaxed);

    {
        std::lock_guard<std::mutex> lk(cbMutex_);
        callbacks_[id] = std::move(cb);
    }

    requestQ_.push(Request{type, id, std::move(snapshot)});
    return id;
}

void MessageBus::requestStop() {
    requestQ_.push(Request{TaskType::Stop, 0, {}});
}

bool MessageBus::deviceWaitRequest(Request& out, std::chrono::milliseconds waitFor) {
    if (stopping_.load(std::memory_order_acquire)) return false;

    Request req;
    const bool got = requestQ_.wait_pop_for(req, waitFor);
    if (!got) return false;

    if (req.type == TaskType::Stop) {
        stopping_.store(true, std::memory_order_release);
        return false;
    }

    out = std::move(req);
    return true;
}

void MessageBus::devicePostResponse(Response r) {
    responseQ_.push(std::move(r));
}

void MessageBus::uiPumpResponses(std::size_t maxBatch) {
    for (std::size_t i = 0; i < maxBatch; ++i) {
        auto opt = responseQ_.try_pop();
        if (!opt) break;

        Response resp = std::move(*opt);

        Callback cb;
        {
            std::lock_guard<std::mutex> lk(cbMutex_);
            auto it = callbacks_.find(resp.id);
            if (it != callbacks_.end()) {
                cb = std::move(it->second);
                callbacks_.erase(it);
            }
        }

        if (cb) cb(resp);
    }
}

bool MessageBus::isStopping() const {
    return stopping_.load(std::memory_order_acquire);
}

void MessageBus::shutdown() {
    bool expected = false;
    if (!shutdownOnce_.compare_exchange_strong(expected, true)) return;

    stopping_.store(true, std::memory_order_release);
    requestQ_.close();
    responseQ_.close();

    std::lock_guard<std::mutex> lk(cbMutex_);
    callbacks_.clear();
}

} // namespace mq
