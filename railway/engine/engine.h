#pragma once

#include <string>
#include "../../api.h"

namespace RailWay {
  class Engine {
  private:
    std::string model;
    int cylinders;

  public:
    explicit Engine(std::string model, int cylinders);

    explicit Engine(InputHandler &handler);

    std::string toString();
  };
}