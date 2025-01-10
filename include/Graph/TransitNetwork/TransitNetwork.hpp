#pragma once

#include <string>

#include "UnitClasses.hpp"

namespace tmn {
namespace transit_network {

class TransitNetwork : public graph::Graph<true, unsigned int, TransferPoint,  Route> {
private:
    Function<double(Route, TransportType, bool)> cost_function;
    Function<unsigned int (Route, TransportType)> time_function;

public:
    TransitNetwork(Function<double(Route, TransportType)>& cost_function, Function<unsigned int (Route, TransportType)>& time_function) : cost_function (cost_function), time_function(time_function) {} 
    ~TransitNetwork() = default;

    
};

}
}