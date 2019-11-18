#include "conductor.h"

using namespace RailWay;

Conductor::Conductor(std::string name) : name(std::move(name)) {}

Conductor::Conductor(InputHandler &handler) {
    this->name = handler.enterString("Enter conductor's name: ");
}

std::string Conductor::toString() {
    return "Conductor (" + name + ")";
}
