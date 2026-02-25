#include "Controller.h"

Controller::Controller(UiBackend& backend)
    : backend_(backend)
{
    connect(&backend_, &UiBackend::snapshotUpdated, this,
            [this](int speed,
                   QString sn1, double v1, double i1, double t1, int soc1,
                   QString sn2, double v2, double i2, double t2, int soc2,
                   double motorCurrent, double motorTemp){
                speed_ = speed;
                motorCurrent_ = motorCurrent;
                motorTemp_ = motorTemp;

                b1Serial_ = sn1; b1Voltage_ = v1; b1Current_ = i1; b1Temp_ = t1; b1Soc_ = soc1;
                b2Serial_ = sn2; b2Voltage_ = v2; b2Current_ = i2; b2Temp_ = t2; b2Soc_ = soc2;

                emit stateChanged();
            });

    connect(&backend_, &UiBackend::operationCompleted, this,
            [this](const QString& msg){
                lastMessage_ = msg;
                emit lastMessageChanged();
            });
}

void Controller::refresh() { backend_.refresh(); }
void Controller::setSpeed(int v) { backend_.setSpeed(v); }
void Controller::setBattery1Serial(const QString& sn) { backend_.setBattery1Serial(sn); }
void Controller::setBattery2Serial(const QString& sn) { backend_.setBattery2Serial(sn); }
