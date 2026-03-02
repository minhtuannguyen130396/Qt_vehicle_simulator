#pragma once

#include <QObject>
#include <QTimer>
#include <QString>

#include "messageQueue/MessageBus.h"

/**
 * UiBackend: UI-facing backend service.
 * - Owns MessageBus response dispatch timer (runs on UI thread).
 * - Provides high-level operations (refresh, setSpeed, set serial...)
 * - Emits signals with updated state for Controller to consume.
 */
class UiBackend : public QObject
{
    Q_OBJECT
public:
    explicit UiBackend(mq::MessageBus& bus, QObject* parent = nullptr);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void setSpeed(int v);
    Q_INVOKABLE void setBattery1Serial(const QString& sn);
    Q_INVOKABLE void setBattery2Serial(const QString& sn);

signals:
    void snapshotUpdated(
        int speed,
        QString sn1, double v1, double i1, double t1, int soc1,
        QString sn2, double v2, double i2, double t2, int soc2,
        double motorCurrent, double motorTemp
    );

    void operationCompleted(QString message);

private:
    mq::MessageBus& bus_;
    QTimer dispatchTimer_;
    QTimer syncTimer_;
};
