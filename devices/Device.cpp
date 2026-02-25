#include "devices/Device.h"

#include <algorithm>

Device::Device(mq::MessageBus& bus, HardwareService& hw)
  : bus_(bus), hw_(hw) {}

Device::~Device() { stop(); }

void Device::start(std::chrono::milliseconds tickPeriod,
                   std::chrono::milliseconds requestWait) {
  tickPeriod_ = tickPeriod;
  requestWait_ = requestWait;

  if (running_.exchange(true)) return;
  worker_ = std::thread([this]{ run_(); });
}

void Device::stop() {
  if (!running_.exchange(false)) return;
  bus_.requestStop();
  if (worker_.joinable()) worker_.join();
  bus_.shutdown();
}

mq::Snapshot Device::makeSnapshot_() const {
  const auto s = hw_.getSnapshot();

  mq::Snapshot out{};
  out.sn1 = s.battery1.serialNumber;
  out.v1  = s.battery1.voltage;
  out.i1  = s.battery1.current;
  out.t1  = s.battery1.temperature;
  out.soc1 = s.battery1.soc;

  out.sn2 = s.battery2.serialNumber;
  out.v2  = s.battery2.voltage;
  out.i2  = s.battery2.current;
  out.t2  = s.battery2.temperature;
  out.soc2 = s.battery2.soc;

  // Map worker RPM to UI scale: for demo keep same value if within 0..100
  out.speed = s.motor.speedRpm;
  out.motorCurrent = s.motor.current;
  out.motorTemp    = s.motor.temperature;

  return out;
}

void Device::run_() {
  using Clock = std::chrono::steady_clock;
  auto nextTick = Clock::now() + tickPeriod_;

  while (running_ && !bus_.isStopping()) {
    // 1) Check incoming request (bounded wait to avoid busy-loop)
    mq::Request req;
    const bool got = bus_.deviceWaitRequest(req, requestWait_);
    if (got) {
      handleRequest_(std::move(req));

      // Small batch drain (best-effort)
      for (int i = 0; i < 8; ++i) {
        mq::Request more;
        if (!bus_.deviceWaitRequest(more, std::chrono::milliseconds(0))) break;
        handleRequest_(std::move(more));
      }
    }

    // 2) Finalize pending "delayed set" operations if ready
    finalizeReadyOps_();

    // 3) Periodic sync
    const auto now = Clock::now();
    if (now >= nextTick) {
      hw_.syncOnce();
      nextTick = now + tickPeriod_;
    }
  }
}

void Device::handleRequest_(mq::Request req) {
  using Clock = std::chrono::steady_clock;

  switch (req.type) {
    case mq::TaskType::GetSnapshot: {
      mq::Response resp;
      resp.id = req.id;
      resp.ok = true;
      resp.snapshot = makeSnapshot_();
      resp.message = "snapshot";
      bus_.devicePostResponse(std::move(resp));
      break;
    }

    case mq::TaskType::SetBattery1Serial: {
      hw_.beginSetBattery1Serial(req.snapshot.sn1);
      pending_.push(PendingOp{Clock::now() + std::chrono::milliseconds(100),
                              req.id, PendingType::SetBattery1Serial});
      break;
    }

    case mq::TaskType::SetBattery2Serial: {
      hw_.beginSetBattery2Serial(req.snapshot.sn2);
      pending_.push(PendingOp{Clock::now() + std::chrono::milliseconds(100),
                              req.id, PendingType::SetBattery2Serial});
      break;
    }

    case mq::TaskType::SetMotorSpeed: {
      hw_.beginSetMotorSpeed(req.snapshot.speed);
      pending_.push(PendingOp{Clock::now() + std::chrono::milliseconds(100),
                              req.id, PendingType::SetMotorSpeed});
      break;
    }

    default: {
      mq::Response resp;
      resp.id = req.id;
      resp.ok = false;
      resp.snapshot = makeSnapshot_();
      resp.message = "Unknown task";
      bus_.devicePostResponse(std::move(resp));
      break;
    }
  }
}

void Device::finalizeReadyOps_() {
  using Clock = std::chrono::steady_clock;
  const auto now = Clock::now();

  while (!pending_.empty() && pending_.top().readyTime <= now) {
    const auto op = pending_.top();
    pending_.pop();

    switch (op.type) {
      case PendingType::SetBattery1Serial:
        hw_.finalizeSetBattery1Serial();
        break;
      case PendingType::SetBattery2Serial:
        hw_.finalizeSetBattery2Serial();
        break;
      case PendingType::SetMotorSpeed:
        hw_.finalizeSetMotorSpeed();
        break;
    }

    mq::Response resp;
    resp.id = op.requestId;
    resp.ok = true;
    resp.snapshot = makeSnapshot_();
    resp.message = "SET completed";
    bus_.devicePostResponse(std::move(resp));
  }
}
