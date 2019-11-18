#pragma once

#include <string>

#include "../conductor/conductor.h"

namespace RailWay {

  class RailwayCarriage {
  private:
    Conductor *conductor;
  public:
    explicit RailwayCarriage(Conductor *cond);
    ~RailwayCarriage() = default;

    std::string toString();
  };
}