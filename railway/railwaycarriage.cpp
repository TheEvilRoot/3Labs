//
// Created by Доктор Кларик on 18.11.19.
//

#include "railwaycarriage.h"

using namespace RailWay;

std::string RailwayCarriage::toString() {
    return "RailwayCarriage\n\t\t" + conductor->toString();
}

RailwayCarriage::RailwayCarriage(RailWay::Conductor *cond) : conductor(cond) {}
