#include "middleware/HardwareService.h"

HardwareService::HardwareService(std::unique_ptr<IBatteryDriver> b1,
                                 std::unique_ptr<IBatteryDriver> b2,
                                 std::unique_ptr<IMotorDriver> motor)
  : b1_(std::move(b1)), b2_(std::move(b2)), motor_(std::move(motor))
{
  // Initialize snapshot immediately
  syncOnce();
}

void HardwareService::syncOnce() {
  std::lock_guard<std::mutex> lk(snapMtx_);
  updateSnapshotLocked_();
}

HwSnapshot HardwareService::getSnapshot() const {
  std::lock_guard<std::mutex> lk(snapMtx_);
  return snapshot_;
}

void HardwareService::beginSetBattery1Serial(std::string serial) {
  b1_->beginSetSerialNumber(std::move(serial));
}

void HardwareService::finalizeSetBattery1Serial() {
  b1_->finalizeSetSerialNumber();
  // Option 1: reflect immediately to snapshot
  std::lock_guard<std::mutex> lk(snapMtx_);
  updateSnapshotLocked_();
}

void HardwareService::beginSetBattery2Serial(std::string serial) {
  b2_->beginSetSerialNumber(std::move(serial));
}

void HardwareService::finalizeSetBattery2Serial() {
  b2_->finalizeSetSerialNumber();
  std::lock_guard<std::mutex> lk(snapMtx_);
  updateSnapshotLocked_();
}

void HardwareService::beginSetMotorSpeed(int rpm) {
  motor_->beginSetTargetSpeed(rpm);
}

void HardwareService::finalizeSetMotorSpeed() {
  motor_->finalizeSetTargetSpeed();
  std::lock_guard<std::mutex> lk(snapMtx_);
  updateSnapshotLocked_();
}

void HardwareService::updateSnapshotLocked_() {
  snapshot_.battery1 = b1_->readStatus();
  snapshot_.battery2 = b2_->readStatus();
  snapshot_.motor = motor_->readStatus();

  auto toSoc = [](float v) -> int {
    // Simple linear mapping for demo: 48V -> 0%, 54V -> 100%
    constexpr float vMin = 48.0f;
    constexpr float vMax = 54.0f;
    float x = (v - vMin) / (vMax - vMin);
    if (x < 0.0f) x = 0.0f;
    if (x > 1.0f) x = 1.0f;
    return static_cast<int>(x * 100.0f + 0.5f);
  };

  snapshot_.battery1.soc = toSoc(snapshot_.battery1.voltage);
  snapshot_.battery2.soc = toSoc(snapshot_.battery2.voltage);
}
