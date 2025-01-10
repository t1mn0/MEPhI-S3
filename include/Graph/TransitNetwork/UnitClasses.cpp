#include "UnitClasses.hpp"

namespace tmn {
namespace transit_network {

std::string to_string(TransferPointType type) {
    switch (type) {
        case TransferPointType::BUS_STOP: return "BUS_STOP";
        case TransferPointType::METRO_STATION: return "METRO_STATION";
        case TransferPointType::AIRPORT: return "AIRPORT";
        case TransferPointType::TRAM_STOP: return "TRAM_STOP";
        case TransferPointType::TROLLEYBUS_STOP: return "TROLLEYBUS_STOP";
        case TransferPointType::TAXI_STAND: return "TAXI_STAND";
        case TransferPointType::DEFAULT_TRANSFER_POINT: return "DEFAULT_TRANSFER_POINT";
        default: return "UNKNOWN_TRANSFER_POINT_TYPE";
    }
}

std::string to_string(TransportType type) {
    switch (type) {
        case TransportType::BUS: return "BUS";
        case TransportType::METRO: return "METRO";
        case TransportType::TRAM: return "TRAM";
        case TransportType::TAXI: return "TAXI";
        case TransportType::WALK: return "WALK";
        case TransportType::BIKE: return "BIKE";
        case TransportType::DEFAULT_TRANSPORT_TYPE: return "DEFAULT_TRANSPORT_TYPE";
        default: return "UNKNOWN_TRANSPORT_TYPE";
    }
}

bool TransferPoint::can_park_here(TransportType type) const {
    return parking_rates.contains(type);
}

double TransferPoint::calculate_parking_cost(TransportType type, int parking_minutes) const {
    if (!can_park_here(type)){
        throw tmn::exception::LogicException("Error(calculate_parking_cost) : this type of transport cannot park at this transfer point: " + to_string(type));
    }
    else{
        return parking_rates[type] * parking_minutes;
    }
}

}
}