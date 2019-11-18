#include "driver.h"

using namespace RailWay;

Driver::Driver(std::string name) : name(std::move(name)) {}

Driver::Driver(InputHandler &handler) {
    this->name = handler.enterString("Enter driver's name: ");
}

std::string Driver::toString() {
    return "Driver (" + name + ")";
}
