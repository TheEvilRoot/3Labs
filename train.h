#pragma once

#include <utility>
#include <vector>
#include <string>
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
  };

  class Driver {
  private:
    std::string name;
  public:
    explicit Driver(std::string name): name(std::move(name)) {}
    explicit Driver(InputHandler& handler) {
        this->name = handler.enterString("Enter driver's name: ");
    }

    friend std::ostream& operator>>(std::ostream& out, Driver& driver) {
        out << "Driver (" << driver.name << ")";
        return out;
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
  };

  class Vehicle {
  private:
    size_t seats;

  public:
    explicit Vehicle(size_t s): seats(s) {}
    ~Vehicle() = default;
  };

  class RailwayCarriage {
  private:
    Conductor* conductor;
  public:
    explicit RailwayCarriage(Conductor* cond): conductor(cond) {}
    ~RailwayCarriage() = default;
  };

  class Locomotive {
  private:
    Driver *driver;
    Engine engine;
  public:
    explicit Locomotive(Driver* driver, Engine engine) : driver(driver), engine(std::move(engine)) {}
    ~Locomotive() = default;
  };

  class Train: public Vehicle {
  private:
    std::vector<RailwayCarriage *> carriages;
    Locomotive* locomotive;
  public:
    explicit Train(size_t seats, Locomotive *loc, std::vector<RailwayCarriage *> cars): Vehicle(seats), locomotive(loc), carriages(cars) {}
    ~Train() = default;
  };

}