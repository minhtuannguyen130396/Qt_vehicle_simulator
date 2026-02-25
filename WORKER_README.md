# Option A Device Worker (C++17 + CMake)

This sample demonstrates **Option A**:
- Single device worker thread
- `condition_variable` timed waiting (`wait_until`) to wake on:
  - incoming requests
  - periodic hardware sync tick
  - completion of delayed hardware "set" operations

## Features
- Two simulated batteries + one simulated motor
- Periodic sync updates a thread-safe snapshot
- `SET` commands are **delayed 100ms** to simulate hardware write time
- After completion, snapshot is updated immediately (**Option 1**) and a response is posted

## Build

```bash
mkdir -p build
cmake -S . -B build
cmake --build build -j
```

## Run

```bash
./build/test_device
```

You should see:
- initial snapshot
- SET responses after ~100ms
- snapshot after SET reflecting new serial/speed
