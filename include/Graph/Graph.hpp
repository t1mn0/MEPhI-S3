#pragma once

#include "../Tuple/Tuple.hpp"
#include "../Associative/HashTable.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "../SmartPtr/SharedPtr.hpp"


using namespace tmn_associative;
using namespace tmn_sequence;
using namespace tmn_smart_ptr;
using namespace tmn_tuple;

namespace tmn_graph {

// provided that I use MY std::function & std::tuple implementations 

// concept or something else: "ResultWeightType is comparable type (<, >, >=, <=, ==)"
template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeightType, typename ... WeightTypes>
class Graph {
private:
// Support structures :
    class Edge {
    private:
        VertexId from;
        VertexId to; 

        std::function<ResultWeight (WeightTypes... weights)> get_result_weight; // calculating the total weight of the edge

        Tuple<WeightTypes...> weights; // different weights of one edge

    public:
        Edge(std::function<ResultWeight (WeightTypes... weights)> updater /* = summa (by default we can try to use summation)*/, WeightTypes... weights);
        
        update(); // updater() method call

        // etc
    };

private:
// Fields :
    // use alias for it 
    HashTable<VertexId, HashTable<VertexId, SharedPtr<Edge>> > adjacency_list;

    void add_edge(VertexId, VertexId, std::function<ResultWeight (WeightTypes... weights)> updater /* by def = summa */, WeightTypes... weights);
    // if our Graph is_oriented == false, we just create 1 edge from VertexId_1 to VertexId_2 and wrap it to SharedPtr 
    //      next :
    //          1) insert in adjacency_list to key VertexId_1
    //          2) insert in adjacency_list to key VertexId_2 (!share it!)
    // else 
    //      just insert in adjacency_list to key VertexId_1

public:
// Constructors & assignment & conversion & destructor:

    void update_edge(VertexId from, VertexId to) // adjacency_list[from][to].update()
    void change_weight_updater(VertexId from, VertexId to, std::function<ResultWeight (WeightTypes... weights)> updater);

};

}

#include "../../src/Graph/Graph.tpp"