#include "board/SimBatteryDriver.h"

#include <chrono>

SimBatteryDriver::SimBatteryDriver(std::string initialSerial)
  : serial_(std::move(initialSerial))
{
  const auto seed = static_cast<unsigned>(
      std::chrono::steady_clock::now().time_since_epoch().count());
  rng_.seed(seed);
}

BatteryStatus SimBatteryDriver::readStatus() {
  BatteryStatus st;
  st.serialNumber = serial_;
  st.voltage = vDist_(rng_);
  st.current = iDist_(rng_);
  st.temperature = tDist_(rng_);
  return st;
}

void SimBatteryDriver::beginSetSerialNumber(std::string serial) {
  pendingSerial_ = std::move(serial);
}

void SimBatteryDriver::finalizeSetSerialNumber() {
  if (pendingSerial_) {
    serial_ = std::move(*pendingSerial_);
    pendingSerial_.reset();
  }
}
