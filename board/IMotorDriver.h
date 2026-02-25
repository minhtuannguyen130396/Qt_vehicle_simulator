#pragma once

#include "middleware/HwTypes.h"

class IMotorDriver {
public:
  virtual ~IMotorDriver() = default;

  virtual MotorStatus readStatus() = 0;

  // Two-phase "set" to simulate hardware delay.
  virtual void beginSetTargetSpeed(int rpm) = 0;
  virtual void finalizeSetTargetSpeed() = 0;
};
