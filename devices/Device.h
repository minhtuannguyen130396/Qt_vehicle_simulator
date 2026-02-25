#pragma once

#include "messageQueue/MessageBus.h"
#include "middleware/HardwareService.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <queue>
#include <thread>
#include <vector>

/**
 * Layer 1: Device worker
 * - Reads requests from mq::MessageBus
 * - Calls middleware (HardwareService)
 * - Posts responses back to mq::MessageBus
 *
 * This runs in its own std::thread (same process as Qt UI).
 */
class Device {
public:
  Device(mq::MessageBus& bus, HardwareService& hw);
  ~Device();

  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;

  // start: create worker thread
  void start(std::chrono::milliseconds tickPeriod,
             std::chrono::milliseconds requestWait);

  // stop: request stop + join
  void stop();

private:
  enum class PendingType {
    SetBattery1Serial,
    SetBattery2Serial,
    SetMotorSpeed,
  };

  struct PendingOp {
    std::chrono::steady_clock::time_point readyTime;
    std::uint64_t requestId;
    PendingType type;
    bool operator>(const PendingOp& other) const { return readyTime > other.readyTime; }
  };

  void run_();
  void handleRequest_(mq::Request req);
  void finalizeReadyOps_();

  mq::Snapshot makeSnapshot_() const;

  mq::MessageBus& bus_;
  HardwareService& hw_;

  std::atomic<bool> running_{false};
  std::chrono::milliseconds tickPeriod_{200};
  std::chrono::milliseconds requestWait_{10};
  std::thread worker_;

  std::priority_queue<PendingOp, std::vector<PendingOp>, std::greater<PendingOp>> pending_;
};
