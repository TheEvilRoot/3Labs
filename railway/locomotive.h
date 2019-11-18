#pragma once

#include "driver.h"
#include "engine.h"
#include "../api.h"

namespace RailWay {

  class Locomotive {
  private:
    Driver *driver;
    Engine engine;
  public:
    explicit Locomotive(Driver *driver, Engine engine);
    explicit Locomotive(std::vector<Driver *> &drivers, InputHandler &handler);
    ~Locomotive() = default;

    std::string toString();
  };
}