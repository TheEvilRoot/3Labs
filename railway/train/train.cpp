#include "train.h"

using namespace RailWay;

Train::Train(Locomotive *loc, std::vector<RailwayCarriage *> &cars) : locomotive(loc) {
    for(auto c : cars) { carriages.push_back(c); }
}

std::string Train::toString() {
    std::ostringstream str;
    str << "Train\n";
    str << "\t" << locomotive->toString() << "\n";

    for (const auto& c : carriages) {
        str << "\t" << c->toString() << "\n";
    }
    return str.str();
}
