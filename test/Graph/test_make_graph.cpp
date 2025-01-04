#include <gtest/gtest.h>
#include <string>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

using namespace tmn::graph;

TEST(MakeGraphTest, MakeCompleteWGraph) {
    auto graph = make_complete_graph<true, int, std::string, int>(10, 0, 144);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(1, "MakeCompleteWGraph", pair.first, pair.second);

    ASSERT_EQ(graph.v_size(), 10);
}

TEST(MakeGraphTest, MakeBipartiteCompleteWGraph) {
    auto graph = make_bipartite_complete_graph<false, int, std::string, int>(3, 6, 0, 3, 9);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(2, "MakeBipartiteCompleteWGraph", pair.first, pair.second);

    ASSERT_EQ(graph.v_size(), 9);
}

TEST(MakeGraphTest, MakeComplementWGraph) {
     ArraySequence<tmn::Pair<tmn::Pair<int, int>, float>> seq = { 
        { {1, 0}, 0.256 }, 
        { {1, 2}, 0.256 },
        { {1, 3}, 0.256 },
        { {1, 4}, 0.256 }, 
        { {2, 0}, 0.512 }, 
        { {2, 1}, 0.512 },
        { {3, 1}, 1.024 },
        { {3, 4}, 1.024 },
        { {4, 4}, 0.02 },
        { {4, 3}, 0.04 },
        { {4, 2}, 0.08 },
        { {4, 1}, 0.16 },
        { {4, 0}, 0.32 },
    };

    Graph<true, int, int, float> orig_graph(seq);

    auto graph = make_complement_graph<true, int, int, float>(orig_graph, 3);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(3, "MakeComplementWGraph", pair.first, pair.second);

    ASSERT_EQ(graph.v_size(), 5);
}

TEST(MakeGraphTest, MakeSimpleChainWGraph) {
    auto graph = make_simple_chain_graph<false, int, char, int>(5, 0);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(2, "MakeSimpleChainWGraph", pair.first, pair.second);

    ASSERT_EQ(graph.v_size(), 5);
}

TEST(MakeGraphTest, MakeWheelWGraph) {
    auto graph = make_wheel_graph<true, int, char, int>(7, 4, 1, 144169);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(2, "MakeWheelWGraph", pair.first, pair.second);

    ASSERT_EQ(graph.v_size(), 7);
}

TEST(MakeGraphTest, Conversation) {
     ArraySequence<tmn::Pair<tmn::Pair<int, int>, float>> seq = { 
        { {1, 0}, 0.256 }, 
        { {1, 2}, 0.256 },
        { {1, 3}, 0.256 },
        { {1, 4}, 0.256 }, 
        { {2, 0}, 0.512 }, 
        { {2, 1}, 0.512 },
        { {3, 1}, 1.024 },
        { {3, 4}, 1.024 },
        { {4, 4}, 0.02 },
        { {4, 3}, 0.04 },
        { {4, 2}, 0.08 },
        { {4, 1}, 0.16 },
        { {4, 0}, 0.32 },
    };

    Graph<true, int, int, float> orig_graph(seq);

    auto unweighted_graph = make_unweighted_graph(orig_graph);
    auto weighted_graph = make_weighted_graph<true, int, int, int>(unweighted_graph);

    ASSERT_EQ(orig_graph.v_size(), 5);
    ASSERT_EQ(unweighted_graph.v_size(), 5);
}