# Test / Usage Example

This folder contains a small runnable example showing how to use the **Device (layer 1)** through the **MessageBus (lib/messageQueue)**.

## What it demonstrates

- Create the `MessageBus`
- Create simulated board drivers (layer 3) and `HardwareService` middleware (layer 2)
- Start the `Device` background worker (Option A: single thread + condition_variable timed-wait)
- Send commands:
  - `GetSnapshot`
  - `SetBattery1Serial`
  - `SetMotorSpeed`
- Observe that **SET responses are returned only after ~100ms**, and that **the snapshot reflects the new value immediately when the SET finishes**.

## Run

From the project root:

```bash
cmake -S . -B build
cmake --build build -j
./build/test_device
```

## How to call "set" and "get"

You call the device via `MessageBus::sendRequest(...)`:

- **Get snapshot**:
  - `cmd = mq::Command::GetSnapshot`
  - `intValue = 0`
  - `strValue = ""`

- **Set battery serial**:
  - `cmd = mq::Command::SetBattery1Serial` or `SetBattery2Serial`
  - `intValue = 0`
  - `strValue = "NEW-SERIAL"`

- **Set motor speed**:
  - `cmd = mq::Command::SetMotorSpeed`
  - `intValue = targetSpeedRpm`
  - `strValue = ""`

Responses arrive asynchronously via the callback you provide to `sendRequest`.
