#include "messageQueue/MessageBus.h"
#include "board/SimBatteryDriver.h"
#include "board/SimMotorDriver.h"
#include "middleware/HardwareService.h"
#include "devices/Device.h"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  MessageBus bus;

  auto b1 = std::make_unique<SimBatteryDriver>("BATT-1-INIT");
  auto b2 = std::make_unique<SimBatteryDriver>("BATT-2-INIT");
  auto motor = std::make_unique<SimMotorDriver>();

  HardwareService hw(std::move(b1), std::move(b2), std::move(motor));

  Device device(bus, hw);
  device.start(std::chrono::milliseconds(200)); // periodic sync

  // 1) Get initial snapshot
  bus.sendRequest(mq::Command::GetSnapshot, 0, "", [](const mq::Response& r){
    std::cout << "[GetSnapshot] ok=" << r.ok << " msg=" << r.message << "\n";
  });

  // 2) Set battery1 serial (response arrives after ~100ms)
  bus.sendRequest(mq::Command::SetBattery1Serial, 0, "BATT-1-NEW-SN", [](const mq::Response& r){
    std::cout << "[SetBattery1Serial] ok=" << r.ok << " msg=" << r.message << "\n";
  });

  // 3) Set motor speed (response arrives after ~100ms)
  bus.sendRequest(mq::Command::SetMotorSpeed, 3500, "", [](const mq::Response& r){
    std::cout << "[SetMotorSpeed] ok=" << r.ok << " msg=" << r.message << "\n";
  });

  // 4) After some time, get snapshot again (should reflect new values immediately after finalize)
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  bus.sendRequest(mq::Command::GetSnapshot, 0, "", [](const mq::Response& r){
    std::cout << "[GetSnapshot after SET] ok=" << r.ok << " msg=" << r.message << "\n";
  });

  // Keep running briefly so callbacks print
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  device.stop();
  return 0;
}
