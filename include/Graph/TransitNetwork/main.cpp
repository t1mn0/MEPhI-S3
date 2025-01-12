#include "TransitNetwork.hpp"

#include <iostream>

using namespace tmn::transit_network;

double cost_function(const Route& route, TransportType type, bool first_entry) {
    double cost = route.base_cost;

    if(route.allowed_transport_types.contains(type)){
        if(route.allowed_transport_types[type].has_value()) {
            cost += route.distance * route.allowed_transport_types[type].value().enterance_cost;
        }
    }

    return cost;
}

unsigned int time_function(const Route& route, TransportType transport_type) {
    unsigned int time = route.distance / (route.max_speed * (1 / route.traffic_level));

    return time;
}

int main(){
    tmn::Function<double (const Route&, TransportType, bool)> first(cost_function);
    tmn::Function<unsigned int (const Route&, TransportType)> second(time_function);
    TransitNetwork object(first, second);
    std::cout << object.size() << std::endl;
    return 0;
}