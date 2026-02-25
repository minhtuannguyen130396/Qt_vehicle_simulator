#include "messageQueue/example/Controller.h"

#include <chrono>
#include <thread>
#include <iostream>

int main() {
    using namespace std::chrono_literals;

    mq::example::Controller controller;
    controller.start();

    controller.sendTaskA("hello A1");
    controller.sendTaskB("hello B1");
    controller.sendTaskA("hello A2");

    // Simulate UI event loop: in Qt, replace this with QTimer calling pumpUi().
    auto t0 = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - t0 < 600ms) {
        controller.pumpUi();
        std::this_thread::sleep_for(10ms);
    }

    controller.stop();
    std::cout << "Done.\n";
    return 0;
}
