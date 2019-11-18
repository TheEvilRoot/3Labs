//
// Created by Доктор Кларик on 18.11.19.
//

#include "locomotive.h"

using namespace RailWay;

RailWay::Locomotive::Locomotive(RailWay::Driver *driver, RailWay::Engine engine) : driver(driver), engine(std::move(engine)) {}

RailWay::Locomotive::Locomotive(std::vector<Driver *> &drivers, InputHandler &handler) : engine(Engine(handler)) {
    // asserts drivers.size() > 0
    if (drivers.size() > 1) {
        std::cout << "Which driver use for this locomotive: \n";
        for (const auto &d : drivers) {
            std::cout << "\t" << d << "\n";
        }
        this->driver = drivers[handler.enterIndexOf("(0-" + std::to_string(drivers.size() - 1) + ") > ", drivers)];
    } else {
        this->driver = drivers[0];
    }
}

std::string RailWay::Locomotive::toString() {
    return "Locomotive \n\t\t" + driver->toString() + "\n\t\t" + engine.toString();
}
