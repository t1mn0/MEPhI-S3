#include <gtest/gtest.h>
#include <string>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

using namespace tmn::graph;

TEST(SingleWeightedGraphTest, DefaultConstructor) {
    Graph<true, char, char, complex_num> graph;
    ASSERT_EQ(graph.v_size(), 0);
}

TEST(SingleWeightedGraphTest, ArraySeqConstructor) {
        
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, complex_num>> seq = { 
        { {'A', 'B'}, complex_num(0, 1) }, // abs = 1
        { {'A', 'C'}, complex_num(2, 2) }, // abs = sqrt(8)
        { {'A', 'D'}, complex_num(5, 12) }, // abs = 13
        { {'D', 'A'}, complex_num(0, 0) }, // abs = 0
        { {'D', 'C'}, complex_num(-1, -1) }, // abs = sqrt(2)
        { {'D', 'E'}, complex_num(11, 3.1313) },
        { {'D', 'F'}, complex_num(0.111, 1.011) },
    };

    Graph<true, char, char, complex_num> graph(seq);

    auto pair = graph.basic_adjacency_list();
    pretty_print_matrix(1, "FT SINGLEW-GRAPH", pair.first, pair.second);
    ASSERT_EQ(graph.v_size(), 6);
    ASSERT_FALSE(graph.pass_weight('A', 'A').has_value());
    ASSERT_FALSE(graph.pass_weight('F', 'E').has_value());
    ASSERT_EQ(graph.v_size(), 6);
    ASSERT_EQ(graph.pass_weight('A', 'B').value().abs(), 1.0);
    ASSERT_EQ(graph.pass_weight('A', 'D').value().abs(), 13.0);
    ASSERT_EQ(graph.pass_weight('D', 'A').value().abs(), 0.0);
}

TEST(SingleWeightedGraphTest, CopyConstructor) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'C', 'D'}, 8192 },
    };

    Graph<false, char, char, int> original(seq);
    Graph copy(original);

    ASSERT_FALSE(copy.oriented());
    ASSERT_FALSE(original.oriented());

    auto original_pair = original.basic_adjacency_list();
    auto copied_pair = copy.basic_adjacency_list();
    pretty_print_matrix(2, "FT SINGLEW-GRAPH", original_pair.first, original_pair.second);
    pretty_print_matrix(3, "FT SINGLEW-GRAPH", copied_pair.first, copied_pair.second);

    ASSERT_EQ(copy.v_size(), 4);
    ASSERT_EQ(original.v_size(), 4);
    ASSERT_TRUE(is_symmetric_matrix(original_pair.first));
    ASSERT_TRUE(is_symmetric_matrix(copied_pair.first));
}

TEST(SingleWeightedGraphTest, MoveConstructor) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'C', 'D'}, 8192 },
    };

    Graph<true, char, char, int> original(seq);
    Graph moved(std::move(original));

    ASSERT_TRUE(moved.oriented());

    auto original_pair = original.basic_adjacency_list();
    auto moved_pair = moved.basic_adjacency_list();
    pretty_print_matrix(4, "FT SINGLEW-GRAPH", original_pair.first, original_pair.second);
    pretty_print_matrix(5, "FT SINGLEW-GRAPH", moved_pair.first, moved_pair.second);

    ASSERT_EQ(moved.v_size(), 4);
    ASSERT_EQ(original.v_size(), 0);
    ASSERT_FALSE(is_symmetric_matrix(moved_pair.first));
}

TEST(SingleWeightedGraphTest, CopyOperator)  {
    
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'C', 'D'}, 8192 },
    };

    Graph<false, char, char, int> original(seq);
    Graph copy(original);

    ASSERT_FALSE(copy.oriented());
    ASSERT_FALSE(original.oriented());

    auto original_pair = original.basic_adjacency_list();
    auto copied_pair = copy.basic_adjacency_list();
    pretty_print_matrix(6, "FT SINGLEW-GRAPH", original_pair.first, original_pair.second);
    pretty_print_matrix(7, "FT SINGLEW-GRAPH", copied_pair.first, copied_pair.second);

    ASSERT_EQ(copy.v_size(), 4);
    ASSERT_EQ(original.v_size(), 4);
    ASSERT_TRUE(is_symmetric_matrix(original_pair.first));
    ASSERT_TRUE(is_symmetric_matrix(copied_pair.first));
}

TEST(SingleWeightedGraphTest, MoveOperator) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'C', 'D'}, 8192 },
    };

    Graph<true, char, char, int> original(seq);
    Graph moved(std::move(original));

    ASSERT_TRUE(moved.oriented());

    auto original_pair = original.basic_adjacency_list();
    auto moved_pair = moved.basic_adjacency_list();
    pretty_print_matrix(4, "FT SINGLEW-GRAPH", original_pair.first, original_pair.second);
    pretty_print_matrix(5, "FT SINGLEW-GRAPH", moved_pair.first, moved_pair.second);

    ASSERT_EQ(moved.v_size(), 4);
    ASSERT_EQ(original.v_size(), 0);
    ASSERT_FALSE(is_symmetric_matrix(moved_pair.first));
}

TEST(SingleWeightedGraphTest, AddVertexWithoutResource) {
    Graph<true, int, std::string, double> graph = { 
        { {1, 0}, 256 }, 
        { {1, 2}, 256 },
        { {1, 3}, 256 },
        { {1, 4}, 256 }, 
        { {2, 0}, 512 }, 
        { {2, 1}, 512 },
        { {3, 1}, 1024 },
        { {3, 4}, 1024 },
        { {4, 4}, 2 },
        { {4, 3}, 4 },
        { {4, 2}, 8 },
        { {4, 1}, 16 },
        { {4, 0}, 32 },
    };

    ASSERT_EQ(graph.v_size(), 5);
    
    try{
        graph.add_vertex(1);
        ASSERT_FALSE(true);
    }
    catch(tmn::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_EQ(graph.v_size(), 5);

    graph.add_vertex(5);
    graph.add_vertex(6);
    graph.add_vertex(7);

    ASSERT_EQ(graph.v_size(), 8);
}

// TEST(SingleWeightedGraphTest, AddVertexWithResource) {
//     Graph<true, int, std::string, void> graph;
//     graph.add_vertex(0, "string 0");
//     graph.add_vertex(1, "string 1");
//     graph.add_vertex(2, "string 2");
//     graph.add_vertex(3, "string 3");
//     graph.add_vertex(4, "string 4");
//     graph.add_vertex(5, "string 5");
//     graph.add_vertex(6, "string 6");
//     graph.add_vertex(7, "string 7");
//     graph.add_vertex(8, "string 8");
//     graph.add_vertex(9, "string 9");
//     ASSERT_EQ(graph.v_size(), 10);

//     ASSERT_EQ(graph.get_resource(0), std::string("string 0"));
//     ASSERT_EQ(graph.get_resource(3), std::string("string 3"));
//     ASSERT_EQ(graph.get_resource(6), std::string("string 6"));
//     ASSERT_EQ(graph.get_resource(8), std::string("string 8"));
//     ASSERT_EQ(graph.get_resource(9), std::string("string 9"));
// }

// TEST(SingleWeightedGraphTest, RemoveVertex) {
//     Graph<true, int, std::string, void> graph;
//     graph.add_vertex(0, "string 0");
//     graph.add_vertex(1, "string 1");
//     graph.add_vertex(2, "string 2");
//     graph.add_vertex(3, "string 3");
//     graph.add_vertex(4, "string 4");
//     graph.add_vertex(5, "string 5");
//     graph.add_vertex(6, "string 6");
//     graph.add_vertex(7, "string 7");
//     graph.add_vertex(8, "string 8");
//     graph.add_vertex(9, "string 9");

//     try {
//         graph.remove_vertex(10);
//         ASSERT_FALSE(true);
//     }
//     catch(tmn_exception::LogicException& e){
//         std::cout << e.what() << std::endl;
//     }
//     ASSERT_EQ(graph.v_size(), 10);

//     graph.remove_vertex(0);
//     graph.remove_vertex(2);
//     graph.remove_vertex(4);
//     graph.remove_vertex(6);
//     graph.remove_vertex(8);
//     ASSERT_EQ(graph.v_size(), 5);

//     ASSERT_EQ(graph.get_resource(1), std::string("string 1"));
//     ASSERT_EQ(graph.get_resource(3), std::string("string 3"));
//     ASSERT_EQ(graph.get_resource(5), std::string("string 5"));
//     ASSERT_EQ(graph.get_resource(7), std::string("string 7"));
//     ASSERT_EQ(graph.get_resource(9), std::string("string 9"));

//     ASSERT_FALSE(graph.vertex_in_graph(0));
//     ASSERT_FALSE(graph.vertex_in_graph(2));
//     ASSERT_FALSE(graph.vertex_in_graph(4));
//     ASSERT_FALSE(graph.vertex_in_graph(6));
//     ASSERT_FALSE(graph.vertex_in_graph(8));

//     try {
//         graph.get_resource(4);
//         ASSERT_FALSE(true);
//     }
//     catch(tmn_exception::LogicException& e){
//         std::cout << e.what() << std::endl;
//     }
// }