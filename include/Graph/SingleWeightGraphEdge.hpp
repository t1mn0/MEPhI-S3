#pragma once

#include "SingleWeightGraph.hpp"

namespace tmn_graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
struct Graph<is_oriented, VertexId, VertexType, Weight>::Edge{
private:
    VertexId from;
    VertexId to; 

    Weight weight;

public:
    explicit Edge(VertexId from, VertexId to, const Weight& weight) noexcept : from(from), to(to), weight(weight) {}
    explicit Edge(VertexId from, VertexId to, Weight&& weight) noexcept : from(from), to(to), weight(std::move(weight)) {}
};

}