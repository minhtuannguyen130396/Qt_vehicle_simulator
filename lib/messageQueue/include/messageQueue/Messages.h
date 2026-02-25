#pragma once

#include <cstdint>
#include <string>

namespace mq {

/**
 * @brief Types of tasks supported by the device worker.
 */
enum class TaskType : std::uint8_t {
    GetSnapshot,
    SetBattery1Serial,
    SetBattery2Serial,
    SetMotorSpeed,
    Stop
};

/**
 * @brief Unified snapshot for UI <-> worker communication.
 * All fields are "latest known" values owned by the worker/middleware.
 */
struct Snapshot {
    // Battery 1
    int soc1{0};
    std::string sn1{"BATT-1-INIT"};
    float v1{0.0f};
    float i1{0.0f};
    float t1{0.0f};

    // Battery 2
    int soc2{0};
    std::string sn2{"BATT-2-INIT"};
    float v2{0.0f};
    float i2{0.0f};
    float t2{0.0f};

    // Motor
    int speed{0};          // UI scale 0..100 (mapped to RPM by worker if needed)
    float motorCurrent{0.0f};
    float motorTemp{0.0f};
};

/**
 * @brief Request message sent from UI/controller to worker thread.
 * For "set" commands, the relevant fields are carried inside snapshot.
 */
struct Request {
    TaskType type{};
    std::uint64_t id{};
    Snapshot snapshot{};
};

/**
 * @brief Response message sent from worker thread back to UI/controller.
 */
struct Response {
    std::uint64_t id{};
    bool ok{};
    Snapshot snapshot{};
    std::string message;
};

} // namespace mq
