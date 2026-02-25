#pragma once

#include <string>
#include <cstdint>

struct BatteryStatus {
  std::string serialNumber;
  float voltage{0.0f};
  float current{0.0f};
  float temperature{0.0f};
  int soc{0};
};

struct MotorStatus {
  int speedRpm{0};
  float current{0.0f};
  float temperature{0.0f};
};

struct HwSnapshot {
  BatteryStatus battery1;
  BatteryStatus battery2;
  MotorStatus motor;

  std::string toString() const;
};
