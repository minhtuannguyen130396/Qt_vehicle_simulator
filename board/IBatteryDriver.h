#pragma once

#include "middleware/HwTypes.h"

#include <string>

class IBatteryDriver {
public:
  virtual ~IBatteryDriver() = default;

  // Periodic read
  virtual BatteryStatus readStatus() = 0;

  // Two-phase "set" to simulate hardware delay.
  virtual void beginSetSerialNumber(std::string serial) = 0;
  virtual void finalizeSetSerialNumber() = 0;
};
