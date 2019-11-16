#pragma once

#include <vector>

namespace Railway {

  class Engine {
  private:
    std::string model;
    std::string type;
    int cylinders;
  };

  class Driver {
  private:
    std::string name;
  };

  class Conductor {
  private:
    std::string name;
  };

  class Vehicle {

  };

  class RailwayCarriage {
    Conductor* conductor;
  };

  class Locomotive: public RailwayCarriage {
    Driver *driver;
    Engine engine;
  };

  class Train: public Vehicle {
  public:
    std::vector<RailwayCarriage *> carriages;
    Locomotive* locomotive;
  };

}