#include "board/SimMotorDriver.h"

#include <chrono>

SimMotorDriver::SimMotorDriver() {
  const auto seed = static_cast<unsigned>(
      std::chrono::steady_clock::now().time_since_epoch().count());
  rng_.seed(seed);
}

MotorStatus SimMotorDriver::readStatus() {
  MotorStatus st;
  st.speedRpm = speedRpm_;
  st.current = iDist_(rng_);
  st.temperature = tDist_(rng_);
  return st;
}

void SimMotorDriver::beginSetTargetSpeed(int rpm) {
  pendingSpeedRpm_ = rpm;
}

void SimMotorDriver::finalizeSetTargetSpeed() {
  if (pendingSpeedRpm_) {
    speedRpm_ = *pendingSpeedRpm_;
    pendingSpeedRpm_.reset();
  }
}
