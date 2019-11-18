#pragma once

#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include "api.h"

namespace Railway {

  class Engine {
  private:
    std::string model;
    int cylinders;

  public:
    Engine(std::string model, int cylinders): model(std::move(model)), cylinders(cylinders) {}
    explicit Engine(InputHandler& handler) {
        this->model = handler.enterString("Enter model of engine: ");
        this->cylinders = handler.handleInput("Enter number of cylinders of engine: ", 1, 100);
    }

    std::string toString() {
        return "Engine (" + model + " " + std::to_string(cylinders) + " cylinders" + ")";

    }
  };

  class Driver {
  private:
    std::string name;
  public:
    explicit Driver(std::string name): name(std::move(name)) {}
    explicit Driver(InputHandler& handler) {
        this->name = handler.enterString("Enter driver's name: ");
    }

    std::string toString() {
        return "Driver (" + name + ")";
    }
  };

  class Conductor {
  private:
    std::string name;

  public:
    explicit Conductor(std::string name): name(std::move(name)) {}
    explicit Conductor(InputHandler& handler) {
        this->name = handler.enterString("Enter conductor's name: ");
    }

    std::string toString() {
        return "Conductor (" + name + ")";
    }
  };

  class Vehicle {
  public:
    explicit Vehicle() {}
    ~Vehicle() = default;
  };

  class RailwayCarriage {
  private:
    Conductor* conductor;
  public:
    explicit RailwayCarriage(Conductor* cond): conductor(cond) {}
    ~RailwayCarriage() = default;

    std::string toString() {
        return "RailwayCarriage\n\t\t" + conductor->toString();
    }
  };

  class Locomotive {
  private:
    Driver *driver;
    Engine engine;
  public:
    explicit Locomotive(Driver* driver, Engine engine) : driver(driver), engine(std::move(engine)) {}
    explicit Locomotive(std::vector<Driver *>& drivers, InputHandler& handler): engine(Engine(handler)) {
        // asserts drivers.size() > 0
        if (drivers.size() > 1){
            std::cout << "Which driver use for this locomotive: \n";
            for (const auto& d : drivers) {
                std::cout << "\t" << d << "\n";
            }
            this->driver = drivers[handler.enterIndexOf("(0-" + std::to_string(drivers.size() - 1) + ") > ", drivers)];
        } else {
            this->driver = drivers[0];
        }
    }
    ~Locomotive() = default;

    std::string toString() {
        return "Locomotive \n\t\t" + driver->toString() + "\n\t\t" + engine.toString();
    }
  };

  class Train: public Vehicle {
  private:
    std::vector<RailwayCarriage *> carriages;
    Locomotive* locomotive;
  public:
    explicit Train(Locomotive *loc, std::vector<RailwayCarriage *>& cars): locomotive(loc) {
        for(auto c : cars) { carriages.push_back(c); }
    }
    ~Train() = default;

    std::string toString() {
        std::ostringstream str;
        str << "Train\n";
        str << "\t" << locomotive->toString() << "\n";

        for (const auto& c : carriages) {
            str << "\t" << c->toString() << "\n";
        }
        return str.str();
    }
  };

}