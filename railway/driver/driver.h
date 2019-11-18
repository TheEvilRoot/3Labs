#pragma once

#include <string>

#include "../../api.h"

namespace RailWay {
  class Driver {
  private:
    std::string name;
  public:
    explicit Driver(std::string name);

    explicit Driver(InputHandler &handler);

    std::string toString();
  };
}