#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "../api.h"
#include "vehicle.h"
#include "railwaycarriage.h"
#include "locomotive.h"

namespace RailWay {

  class Train: public Vehicle {
  private:
    std::vector<RailwayCarriage *> carriages;
    Locomotive* locomotive;
  public:
    explicit Train(Locomotive *loc, std::vector<RailwayCarriage *>& cars);
    ~Train() = default;

    std::string toString();
  };

}