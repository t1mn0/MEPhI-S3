#pragma once

#include <string>
#include <limits>

#include "../Graph.hpp"
#include "../../Function/Function.hpp"
#include "../../Tuple/Tuple.hpp"

namespace tmn {
namespace transit_network {

enum class TransferPointType {
    BUS_STOP,
    METRO_STATION,
    AIRPORT,
    TRAM_STOP,
    TROLLEYBUS_STOP,
    TAXI_STAND,
    CROSSROAD,
    DEFAULT_TRANSFER_POINT
};

enum class TransportType {
    BUS,
    METRO,
    TRAM,
    TAXI,
    WALK,
    BIKE,
    DEFAULT_TRANSPORT_TYPE
};

std::string to_string(TransferPointType type);
std::string to_string(TransportType type);

class TransferPoint {
public:
    int x_coordinate = 0;
    int y_coordinate = 0;
    std::string name = "unknown TransferPoint";
    TransferPointType type = TransferPointType::DEFAULT_TRANSFER_POINT;
   
    HashTable<TransportType, double> parking_rates;

public:
    TransferPoint() = default;
    TransferPoint(int x, int y, const std::string& name, TransferPointType type) : x_coordinate(x), y_coordinate(y), name(name), type(type) {};
    ~TransferPoint() = default;

    bool can_park_here(TransportType type) const;
    double calculate_parking_cost(TransportType type, int parking_minutes) const;
};

class Schedule {
public:
    double enterance_cost;
    unsigned int start_route_id;
    unsigned int end_route_id;
    unsigned int current_route_id;
    sequence::ArraySequence<int> departure_times;
    sequence::ArraySequence<int> arrival_times;

    Schedule() = default;
    Schedule(double enterance_cost, unsigned int start_route_id, unsigned int end_route_id, unsigned int current_route_id) 
        : enterance_cost(enterance_cost), start_route_id(start_route_id), end_route_id(end_route_id), current_route_id(current_route_id) {}
    ~Schedule() = default;
};

class Route {
public:
    double distance = 0;
    double base_cost = 0; // route can be paid (per minute)
    double max_speed = 40;
    unsigned int traffic_level = 1; // [1; 10] 

    HashTable<TransportType, Optional<Schedule>> allowed_transport_types;

    Route() = default;
    Route(double dist, double cost, double max_speed) : distance(dist), base_cost(cost), max_speed(max_speed) {}
    ~Route() = default;
};

struct PathInfo {
   double total_cost = std::numeric_limits<double>::max();
   unsigned int total_time = std::numeric_limits<unsigned int>::max();
   
   ArraySequence<unsigned int> path;
   
   PathInfo() = default;
   PathInfo(double cost, unsigned int time, const ArraySequence<unsigned int>& p) : total_cost(cost), total_time(time), path(p) {}

   bool operator < (const PathInfo& other) const {
      if (total_time < other.total_time) {
        return true;
      }
      if (total_time == other.total_time && total_cost < other.total_cost) {
        return true;
      }
      
      return false;
   }
};

}
}