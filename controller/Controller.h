#pragma once

#include <QObject>
#include <QString>

#include "../ui/UiBackend.h"

class Controller : public QObject
{
    Q_OBJECT

    // Motor
    Q_PROPERTY(int speed READ speed NOTIFY stateChanged)
    Q_PROPERTY(double motorCurrent READ motorCurrent NOTIFY stateChanged)
    Q_PROPERTY(double motorTemp READ motorTemp NOTIFY stateChanged)

    // Battery 1
    Q_PROPERTY(QString b1Serial READ b1Serial NOTIFY stateChanged)
    Q_PROPERTY(double b1Voltage READ b1Voltage NOTIFY stateChanged)
    Q_PROPERTY(double b1Current READ b1Current NOTIFY stateChanged)
    Q_PROPERTY(double b1Temp READ b1Temp NOTIFY stateChanged)
    Q_PROPERTY(int b1Soc READ b1Soc NOTIFY stateChanged)

    // Battery 2
    Q_PROPERTY(QString b2Serial READ b2Serial NOTIFY stateChanged)
    Q_PROPERTY(double b2Voltage READ b2Voltage NOTIFY stateChanged)
    Q_PROPERTY(double b2Current READ b2Current NOTIFY stateChanged)
    Q_PROPERTY(double b2Temp READ b2Temp NOTIFY stateChanged)
    Q_PROPERTY(int b2Soc READ b2Soc NOTIFY stateChanged)

    // Last operation message
    Q_PROPERTY(QString lastMessage READ lastMessage NOTIFY lastMessageChanged)

public:
    explicit Controller(UiBackend& backend);

    int speed() const { return speed_; }
    double motorCurrent() const { return motorCurrent_; }
    double motorTemp() const { return motorTemp_; }

    QString b1Serial() const { return b1Serial_; }
    double b1Voltage() const { return b1Voltage_; }
    double b1Current() const { return b1Current_; }
    double b1Temp() const { return b1Temp_; }
    int b1Soc() const { return b1Soc_; }

    QString b2Serial() const { return b2Serial_; }
    double b2Voltage() const { return b2Voltage_; }
    double b2Current() const { return b2Current_; }
    double b2Temp() const { return b2Temp_; }
    int b2Soc() const { return b2Soc_; }

    QString lastMessage() const { return lastMessage_; }

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void setSpeed(int v);
    Q_INVOKABLE void setBattery1Serial(const QString& sn);
    Q_INVOKABLE void setBattery2Serial(const QString& sn);

signals:
    void stateChanged();
    void lastMessageChanged();

private:
    UiBackend& backend_;

    int speed_{0};
    double motorCurrent_{0.0};
    double motorTemp_{0.0};

    QString b1Serial_{};
    double b1Voltage_{0.0};
    double b1Current_{0.0};
    double b1Temp_{0.0};
    int b1Soc_{0};

    QString b2Serial_{};
    double b2Voltage_{0.0};
    double b2Current_{0.0};
    double b2Temp_{0.0};
    int b2Soc_{0};

    QString lastMessage_{};
};
