
#include "Device.h"
#include <thread>
#include <QDebug>
#include <iostream>
Device::Device(MessageBus& bus) : bus_(bus) {}

void Device::start(std::chrono::milliseconds p,std::chrono::milliseconds requestWait)
{
    period_ = p;
    requestWait_= requestWait;
    running_ = true;
    th_ = std::thread(&Device::loop, this);
}

void Device::stop()
{
    if(!running_.exchange(false)) return;
    bus_.requestStop(); //Send command stop to break loop even.
    if (th_.joinable()) th_.join();
    bus_.shutdown(); // Clean resources.
}


void Device::loop()
{
    while (running_) {
        // Non-blocking: poll and drain request queue
        mq::Request req;
        const bool got = bus_.deviceWaitRequest(req, requestWait_);
        if (got) {

            mq::Response resp;
            resp.id = req.id;
            resp.ok = true;
            // handle request
            switch (req.type) {
            case mq::TaskType::GetSnapshot:
                resp.snapshot = snapshot_;
                resp.message = "snapshot";
                break;

            case mq::TaskType::SetBattery1Serial:
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                snapshot_.sn1 = req.snapshot.sn1;
                resp.message = "Battery1 serial updated";
                break;

            case mq::TaskType::SetBattery2Serial:
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                snapshot_.sn2 = req.snapshot.sn2;
                resp.message = "Battery2 serial updated";
                break;

            case mq::TaskType::SetMotorSpeed:
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                snapshot_.speed = req.snapshot.speed;
                resp.message = "Motor speed updated";
                break;
            }
            qDebug() <<"found message come, message: "<<resp.message;
            bus_.devicePostResponse(std::move(resp));
        }

        // Simulated periodic sync update
        snapshot_.soc1 = (snapshot_.soc1 + 1) % 101;
        snapshot_.soc2 = (snapshot_.soc2 + 2) % 101;

        std::this_thread::sleep_for(period_);
    }
}
