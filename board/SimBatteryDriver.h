#pragma once

#include "board/IBatteryDriver.h"

#include <optional>
#include <random>
#include <string>

class SimBatteryDriver final : public IBatteryDriver {
public:
  explicit SimBatteryDriver(std::string initialSerial);

  BatteryStatus readStatus() override;

  void beginSetSerialNumber(std::string serial) override;
  void finalizeSetSerialNumber() override;

private:
  std::string serial_;
  std::optional<std::string> pendingSerial_;

  std::mt19937 rng_;
  std::uniform_real_distribution<float> vDist_{48.0f, 54.0f};
  std::uniform_real_distribution<float> iDist_{-10.0f, 30.0f};
  std::uniform_real_distribution<float> tDist_{20.0f, 60.0f};
};
