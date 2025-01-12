#pragma once

#include <string>

#include "UnitClasses.hpp"

namespace tmn {
namespace transit_network {

class TransitNetwork : protected graph::Graph<true, unsigned int, TransferPoint, Route> {
private:
    Function<double (const Route&, TransportType, bool)> cost_function;
    Function<unsigned int (const Route&, TransportType)> time_function;

public:
    TransitNetwork(const Function<double(const Route&, TransportType, bool)>& cost_function, const Function<unsigned int (const Route&, TransportType)>& time_function) : 
        cost_function (cost_function), time_function(time_function) {} 
    ~TransitNetwork() = default;

    void add_transfer_point(unsigned int id, const TransferPoint& point);
    void add_transfer_point(unsigned int id, TransferPoint&& point);
    void remove_transfer_point(unsigned int id);
    std::size_t size() const noexcept;

    const TransferPoint& get_transfer_point_info(unsigned int id) const;
    TransferPoint& get_transfer_point_info(unsigned int id);
    void change_transfer_point_info(unsigned int id, const TransferPoint& point);
    void change_transfer_point_info(unsigned int id, TransferPoint&& point);

    void update_transfer_point_coordinates(unsigned int id, int x_coordinate, int y_coordinate);
    void update_transfer_point_type(unsigned int id, TransferPointType type);
    void update_transfer_point_parking_rates(unsigned int id, const HashTable<TransportType, double>& parking_rates);

    void add_route(unsigned int from, unsigned int to, const Route& route);
    void add_route(unsigned int from, unsigned int to, Route&& route);
    void remove_route(unsigned int from, unsigned int to);
    bool has_route_between(unsigned int from, unsigned int to) const;

    double passage_cost(unsigned int from, unsigned int to) const noexcept;
    unsigned int passage_time(unsigned int from, unsigned int to) const noexcept;

    void change_route(unsigned int from, unsigned int to, const Route& route) noexcept;
    void change_route(unsigned int from, unsigned int to, Route&& route) noexcept;

    void update_route_distance(unsigned int from, unsigned int to, double distance);
    void update_route_base_cost(unsigned int from, unsigned int to, double cost);
    void update_route_max_speed(unsigned int from, unsigned int to, double max_speed);
    void update_route_traffic_level(unsigned int from, unsigned int to, double max_speed);

    ArraySequence<unsigned int>  find_optimal_path(unsigned int start, unsigned int end, Optional<TransportType> transport_type = Optional<TransportType>());
};

}
}