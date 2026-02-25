#pragma once

#include "board/IMotorDriver.h"

#include <optional>
#include <random>

class SimMotorDriver final : public IMotorDriver {
public:
  SimMotorDriver();

  MotorStatus readStatus() override;

  void beginSetTargetSpeed(int rpm) override;
  void finalizeSetTargetSpeed() override;

private:
  int speedRpm_{0};
  std::optional<int> pendingSpeedRpm_;

  std::mt19937 rng_;
  std::uniform_real_distribution<float> iDist_{0.0f, 25.0f};
  std::uniform_real_distribution<float> tDist_{25.0f, 80.0f};
};
