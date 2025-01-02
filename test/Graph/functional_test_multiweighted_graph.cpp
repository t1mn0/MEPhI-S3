#include <gtest/gtest.h>
#include <string>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

using namespace tmn::graph;

TEST(MultiWeightedGraphTest, DefaultConstructor) {
    Graph<true, char, char, int, RoadStatus, float, int> graph;
    ASSERT_EQ(graph.v_size(), 0);
}