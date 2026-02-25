#include "middleware/HwTypes.h"

#include <sstream>

std::string HwSnapshot::toString() const {
  std::ostringstream oss;
  oss << "Battery1{serial=" << battery1.serialNumber
      << ", V=" << battery1.voltage
      << ", I=" << battery1.current
      << ", T=" << battery1.temperature
      << ", SOC=" << battery1.soc
      << "} "
      << "Battery2{serial=" << battery2.serialNumber
      << ", V=" << battery2.voltage
      << ", I=" << battery2.current
      << ", T=" << battery2.temperature
      << ", SOC=" << battery2.soc
      << "} "
      << "Motor{speed=" << motor.speedRpm
      << ", I=" << motor.current
      << ", T=" << motor.temperature
      << "}";
  return oss.str();
}
