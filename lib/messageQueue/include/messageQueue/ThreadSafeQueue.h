#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>
#include <chrono>

namespace mq {

/**
 * @brief A minimal thread-safe queue supporting:
 *  - push(T)
 *  - try_pop() (non-blocking)
 *  - wait_pop_for(out, timeout) (timed blocking)
 *  - close() (unblocks waiters; further push() is ignored)
 *
 * Threading model:
 *  - Multiple producers are supported.
 *  - Multiple consumers are supported, but in this demo we use single-consumer per queue.
 *
 * Return rules:
 *  - try_pop(): std::nullopt if empty.
 *  - wait_pop_for(): false if timeout OR closed+empty; true if an item was popped.
 */
template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    /**
     * @brief Push an item into the queue.
     * @param v Item to push (moved into the queue).
     * @note If queue is closed, push() is ignored.
     */
    void push(T v) {
        {
            std::lock_guard<std::mutex> lk(m_);
            if (closed_) return;
            q_.push(std::move(v));
        }
        cv_.notify_one();
    }

    /**
     * @brief Non-blocking pop.
     * @return std::optional<T> containing an item if available, otherwise std::nullopt.
     */
    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lk(m_);
        if (q_.empty()) return std::nullopt;
        T v = std::move(q_.front());
        q_.pop();
        return v;
    }

    /**
     * @brief Timed wait pop.
     * @param out Output item if popped.
     * @param timeout Duration to wait.
     * @return true if an item was popped; false if timeout OR closed+empty.
     */
    template <class Rep, class Period>
    bool wait_pop_for(T& out, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait_for(lk, timeout, [&]{ return closed_ || !q_.empty(); });
        if (q_.empty()) return false;
        out = std::move(q_.front());
        q_.pop();
        return true;
    }

    /**
     * @brief Close the queue.
     * Unblocks any waiting consumers; subsequent push() calls are ignored.
     */
    void close() {
        {
            std::lock_guard<std::mutex> lk(m_);
            closed_ = true;
        }
        cv_.notify_all();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> q_;
    bool closed_{false};
};

} // namespace mq
