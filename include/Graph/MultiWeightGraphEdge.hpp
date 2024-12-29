#pragma once

#include "MultiWeightGraph.hpp"

namespace tmn {

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
struct Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::Edge{
private:
    VertexId from;
    VertexId to; 

    Tuple<WeightTypes...> weights;

public:
    explicit Edge(VertexId from, VertexId to, WeightTypes... weights) noexcept : from(from), to(to), weights(weights...) {}
};

}