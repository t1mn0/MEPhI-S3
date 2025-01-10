#include "TransitNetwork.hpp"

#include <iostream>

double cost_function(const Route& route, TransportType type, bool first_entry = true) {
    double cost = route.base_cost;

    if(route.allowed_transport_types.contains(type)){
        if(route.allowed_transport_types[type].has_value()) {
            cost += route.distance * route.allowed_transport_types[type].value().enterance_cost;
        }
    }

    return cost;
}

int main(){
    tmn::transit_network::TransitNetwork object;
    std::cout << object.v_size() << std::endl;
    return 0;
}