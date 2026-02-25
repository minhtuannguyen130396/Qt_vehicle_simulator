#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controller/Controller.h"
#include "ui/UiBackend.h"

#include "messageQueue/MessageBus.h"
#include "devices/Device.h"
#include "board/SimBatteryDriver.h"
#include "board/SimMotorDriver.h"
#include "middleware/HardwareService.h"

#include <memory>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    mq::MessageBus bus;

    // Worker hardware stack (device -> middleware -> simulator board)
    auto b1 = std::make_unique<SimBatteryDriver>("BATT-1-INIT");
    auto b2 = std::make_unique<SimBatteryDriver>("BATT-2-INIT");
    auto motor = std::make_unique<SimMotorDriver>();
    HardwareService hw(std::move(b1), std::move(b2), std::move(motor));

    Device device(bus, hw);
    device.start(std::chrono::milliseconds(200), std::chrono::milliseconds(10));

    UiBackend backend(bus);
    Controller controller(backend);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controller);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    const int ret = app.exec();

    device.stop();
    return ret;
}
