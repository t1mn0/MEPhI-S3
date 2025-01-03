#pragma once

#include <string>

#include "Graph.hpp"
#include "../Function/Function.hpp"
#include "../Tuple/Tuple.hpp"

namespace tmn {

enum class TransferPointType {
    BUS_STOP,
    METRO_STATION,
    TRAIN_STATION,
    AIRPORT,
    OTHER
};

class TransferPoint {
private:
    int x_coordinate = 0;
    int y_coordinate = 0;
    std::string name = "unknown";
    TransferPointType type = TransferPointType::OTHER;
public:
    TransferPoint() = default;
    TransferPoint(int x, int y, const std::string& name, TransferPointType type) : x_coordinate(x), y_coordinate(y), id(id), type(type) {};
    ~TransferPoint() = default;

    int get_x() const noexcept;
    int get_y() const noexcept;
    const std::string& get_name() const noexcept;
    TransferPointType get_type() const noexcept;

    void set_x(int x) noexcept;
    void get_y(int y) noexcept;
    void set_name(const std::string& new_name) noexcept;
    void set_type(TransferPoint new_type) noexcept;
};

// --------------------------------------> TRANSIT NETWORK <--------------------------------------

class TransitNetwork {
private:
    graph::Graph<true, unsigned int, TransferPoint, Tuple<>>
};

}