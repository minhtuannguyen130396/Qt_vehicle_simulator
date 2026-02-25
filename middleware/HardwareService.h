#pragma once

#include "board/IBatteryDriver.h"
#include "board/IMotorDriver.h"
#include "middleware/HwTypes.h"

#include <memory>
#include <mutex>
#include <string>

// Layer 2: Middleware / service
// - Owns drivers
// - Owns snapshot (thread-safe)
// - Provides begin/finalize operations for delayed "set" commands

class HardwareService {
public:
  HardwareService(std::unique_ptr<IBatteryDriver> b1,
                  std::unique_ptr<IBatteryDriver> b2,
                  std::unique_ptr<IMotorDriver> motor);

  // Periodic sync
  void syncOnce();

  // Immediate snapshot read (copy)
  HwSnapshot getSnapshot() const;

  // Two-phase set APIs
  void beginSetBattery1Serial(std::string serial);
  void finalizeSetBattery1Serial();

  void beginSetBattery2Serial(std::string serial);
  void finalizeSetBattery2Serial();

  void beginSetMotorSpeed(int rpm);
  void finalizeSetMotorSpeed();

private:
  void updateSnapshotLocked_();

  std::unique_ptr<IBatteryDriver> b1_;
  std::unique_ptr<IBatteryDriver> b2_;
  std::unique_ptr<IMotorDriver> motor_;

  mutable std::mutex snapMtx_;
  HwSnapshot snapshot_;
};
