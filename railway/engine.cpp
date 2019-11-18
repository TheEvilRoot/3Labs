#include "engine.h"

using namespace RailWay;

Engine::Engine(InputHandler &handler) {
    this->model = handler.enterString("Enter model of engine: ");
    this->cylinders = handler.handleInput("Enter number of cylinders of engine: ", 1, 100);
}

Engine::Engine(std::string model, int cylinders) : model(std::move(model)), cylinders(cylinders) {}

std::string Engine::toString() {
    return "Engine (" + model + " " + std::to_string(cylinders) + " cylinders" + ")";

}
