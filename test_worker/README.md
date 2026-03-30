# Test / Usage Example

This folder contains a small runnable example that shows how to use the **Device (layer 1)** through the **MessageBus** in [`lib/messageQueue/`](E:\Qt_vehicle_simulator\lib\messageQueue).

## What It Demonstrates

- Creating a `MessageBus`
- Creating simulated board drivers (layer 3) and the `HardwareService` middleware (layer 2)
- Starting the `Device` background worker
- Running the worker with the Option A model:
  - single worker thread
  - `condition_variable` timed wait
- Sending commands asynchronously:
  - `GetSnapshot`
  - `SetBattery1Serial`
  - `SetMotorSpeed`
- Observing delayed `SET` completion:
  - responses are returned after about `100 ms`
  - the snapshot is updated as soon as the delayed write finishes

## Run

From the project root:

```bash
cmake -S . -B build
cmake --build build -j
./build/test_device
```

## Interaction Model

The worker is called through `MessageBus`, not directly through the hardware layer.

- `GetSnapshot`
  - Requests the latest simulated hardware snapshot
- `SetBattery1Serial`
  - Schedules a delayed update for battery 1 serial number
- `SetMotorSpeed`
  - Schedules a delayed update for motor speed

Responses are delivered asynchronously through the callback registered on the request.

## Why This Folder Exists

This example isolates the worker and queue flow from the Qt/QML UI so the threading model, delayed operations, and request/response behavior can be validated independently.
