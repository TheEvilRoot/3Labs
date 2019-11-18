#pragma once

#include <string>

#include "../api.h"

namespace RailWay {

    class Conductor {
    private:
      std::string name;

    public:
      explicit Conductor(std::string name);
      explicit Conductor(InputHandler& handler);

      std::string toString();
    };
}