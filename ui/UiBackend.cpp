#include "UiBackend.h"

#include <QDebug>

UiBackend::UiBackend(mq::MessageBus& bus, QObject* parent)
    : QObject(parent)
    , bus_(bus)
{
    // Drain responses periodically on UI thread (non-blocking)
    connect(&dispatchTimer_, &QTimer::timeout, this, [this]{
        bus_.uiPumpResponses(16);
    });
    dispatchTimer_.start(16); // ~60fps
}

void UiBackend::refresh()
{
    mq::Snapshot payload{};
    bus_.enqueueRequest(mq::TaskType::GetSnapshot, std::move(payload), [this](const mq::Response& r){
        emit snapshotUpdated(
            r.snapshot.speed,
            QString::fromStdString(r.snapshot.sn1), r.snapshot.v1, r.snapshot.i1, r.snapshot.t1, r.snapshot.soc1,
            QString::fromStdString(r.snapshot.sn2), r.snapshot.v2, r.snapshot.i2, r.snapshot.t2, r.snapshot.soc2,
            r.snapshot.motorCurrent, r.snapshot.motorTemp
        );
    });
}

void UiBackend::setSpeed(int v)
{
    mq::Snapshot payload{};
    payload.speed = v;

    bus_.enqueueRequest(mq::TaskType::SetMotorSpeed, std::move(payload), [this](const mq::Response& r){
        emit snapshotUpdated(
            r.snapshot.speed,
            QString::fromStdString(r.snapshot.sn1), r.snapshot.v1, r.snapshot.i1, r.snapshot.t1, r.snapshot.soc1,
            QString::fromStdString(r.snapshot.sn2), r.snapshot.v2, r.snapshot.i2, r.snapshot.t2, r.snapshot.soc2,
            r.snapshot.motorCurrent, r.snapshot.motorTemp
        );
        emit operationCompleted(QString::fromStdString(r.message));
    });
}

void UiBackend::setBattery1Serial(const QString& sn)
{
    mq::Snapshot payload{};
    payload.sn1 = sn.toStdString();

    bus_.enqueueRequest(mq::TaskType::SetBattery1Serial, std::move(payload), [this](const mq::Response& r){
        emit snapshotUpdated(
            r.snapshot.speed,
            QString::fromStdString(r.snapshot.sn1), r.snapshot.v1, r.snapshot.i1, r.snapshot.t1, r.snapshot.soc1,
            QString::fromStdString(r.snapshot.sn2), r.snapshot.v2, r.snapshot.i2, r.snapshot.t2, r.snapshot.soc2,
            r.snapshot.motorCurrent, r.snapshot.motorTemp
        );
        emit operationCompleted(QString::fromStdString(r.message));
    });
}

void UiBackend::setBattery2Serial(const QString& sn)
{
    mq::Snapshot payload{};
    payload.sn2 = sn.toStdString();

    bus_.enqueueRequest(mq::TaskType::SetBattery2Serial, std::move(payload), [this](const mq::Response& r){
        emit snapshotUpdated(
            r.snapshot.speed,
            QString::fromStdString(r.snapshot.sn1), r.snapshot.v1, r.snapshot.i1, r.snapshot.t1, r.snapshot.soc1,
            QString::fromStdString(r.snapshot.sn2), r.snapshot.v2, r.snapshot.i2, r.snapshot.t2, r.snapshot.soc2,
            r.snapshot.motorCurrent, r.snapshot.motorTemp
        );
        emit operationCompleted(QString::fromStdString(r.message));
    });
}
