#include <gtest/gtest.h>
#include <string>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

using namespace tmn_graph;

TEST(SingleWeightedGraphTest, DefaultConstructor) {
    Graph<true, char, char, complex_num> graph;
    ASSERT_EQ(graph.v_size(), 0);
}

TEST(SingleWeightedGraphTest, ArraySeqConstructor) {
        
    tmn_sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, complex_num>> seq = { 
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

    // Graph<true, char, char, void> graph2 = { EdgeChar('A', 'A'), EdgeChar('B', 'B'), EdgeChar('C', 'C') };
    // auto pair2 = graph2.basic_adjacency_list();
    // pretty_print_matrix(2, "FT SINGLEW-GRAPH", pair2.first, pair2.second);
    // ASSERT_EQ(graph2.v_size(), 3);

    // Graph<false, char, char, void> graph3 = { 
    //     EdgeChar('A', 'B'), EdgeChar('A', 'C'), EdgeChar('A', 'D'), EdgeChar('A', 'E'), EdgeChar('A', 'F'), 
    //     EdgeChar('A', 'G'), EdgeChar('A', 'H'), EdgeChar('A', 'I'), EdgeChar('A', 'J'), EdgeChar('A', 'K'), 
    //     EdgeChar('A', 'L'), EdgeChar('A', 'M'), EdgeChar('A', 'N'), EdgeChar('A', 'O'), EdgeChar('A', 'P'), 
    //     EdgeChar('A', 'Q'), EdgeChar('A', 'R'), EdgeChar('A', 'S'), EdgeChar('A', 'T'), EdgeChar('A', 'U'), 
    //     EdgeChar('A', 'V'), EdgeChar('A', 'W'), EdgeChar('A', 'X'), EdgeChar('A', 'Y'), EdgeChar('A', 'Z') };
    // ASSERT_EQ(graph3.v_size(), 26);
    // auto pair3 = graph3.basic_adjacency_list();
    // pretty_print_matrix(3, pair3.first, pair3.second);
    // ASSERT_TRUE(is_symmetric_matrix(pair3.first));

    // Graph<true, double, double, void> graph4 = {  };
    // auto pair4 = graph4.basic_adjacency_list();
    // pretty_print_matrix(4, pair4.first, pair4.second);
    // ASSERT_EQ(graph4.v_size(), 0);

    // Graph<false, int, double, void> graph5 = { EdgeInt(1, 2), EdgeInt(2, 3), EdgeInt(4, 5), EdgeInt(6, 7), EdgeInt(8, 9), EdgeInt(9, 0) };
    // auto pair5 = graph5.basic_adjacency_list();
    // pretty_print_matrix(5, pair5.first, pair5.second);
    // ASSERT_EQ(graph5.v_size(), 10);
    // ASSERT_TRUE(is_symmetric_matrix(pair5.first));
}

// TEST(SingleWeightedGraphTest, InitializerList2) {
    
//     Graph<true, char, char, void> graph1 = { 
//         {'A', {'B', 'G', 'D'}},
//         {'B', {'C', 'D'}},
//         {'C', {'B', 'A'}},
//         {'D', {'G'}}
//     };
//     //          _______
//     //         V      \           A B C D G
//     // G <--- D <--   |         A     1      
//     // ^   ______/    |         B 1   1
//     // |  /   ________|   =>    C   1
//     // | /   /                  D 1 1       
//     // A -> B <-> С --\         G 1     1
//     // ^              | 
//     //  \_____________|

//     auto pair1 = graph1.basic_adjacency_list();
//     pretty_print_matrix(6, pair1.first, pair1.second);
//     ASSERT_EQ(graph1.v_size(), 5);



//     Graph<false, int, double, void> graph2 = { 
//         {1, {2, 3, 4}},
//         {2, {3, 4}},
//         {3, {1}}
//     };

//     auto pair2 = graph2.basic_adjacency_list();
//     pretty_print_matrix(7, pair2.first, pair2.second);
//     ASSERT_EQ(graph2.v_size(), 4);
//     ASSERT_TRUE(is_symmetric_matrix(pair2.first));
// }

// TEST(SingleWeightedGraphTest, CopyConstructor) {
//     Graph<true, char, char, void> original = { 
//         {'A', {'B', 'C'}},
//         {'B', {'A', 'C'}},
//         {'C', {'A', 'B'}}
//     };

//     Graph<true, char, char, void> copy(original);

//     auto original_pair = original.basic_adjacency_list();
//     auto copied_pair = copy.basic_adjacency_list();
//     pretty_print_matrix(8, original_pair.first, original_pair.second);
//     pretty_print_matrix(9, copied_pair.first, copied_pair.second);

//     ASSERT_EQ(copy.v_size(), 3);
//     ASSERT_EQ(original.v_size(), 3);
//     ASSERT_TRUE(is_symmetric_matrix(original_pair.first));
//     ASSERT_TRUE(is_symmetric_matrix(copied_pair.first));
// }

// TEST(SingleWeightedGraphTest, MoveConstructor) {
//     Graph<true, char, char, void> original = { 
//         {'A', {'B', 'C'}},
//         {'B', {'A', 'C'}},
//         {'C', {'A', 'B'}}
//     };

//     Graph<true, char, char, void> moved(std::move(original));

//     auto original_pair = original.basic_adjacency_list();
//     auto moved_pair = moved.basic_adjacency_list();
//     pretty_print_matrix(10, original_pair.first, original_pair.second);
//     pretty_print_matrix(11, moved_pair.first, moved_pair.second);
    
//     ASSERT_EQ(moved.v_size(), 3);
//     ASSERT_EQ(original.v_size(), 0);
//     ASSERT_TRUE(is_symmetric_matrix(moved_pair.first));
// }

// TEST(SingleWeightedGraphTest, CopyOperator) {
//     Graph<false, int, char, void> graph1 = { 
//         {1, {2, 3}},
//         {2, {4}   },
//         {3, {5, 4}},
//         {6, {1, 2, 3, 4, 5}}
//     };

//     Graph<false, int, char, void> graph2 = { 
//         {1, {2}   },
//         {2, {3}   },
//         {0, {1, 2} }
//     };

//     ASSERT_EQ(graph1.v_size(), 6);
//     ASSERT_EQ(graph2.v_size(), 4);
    
//     graph2 = graph1;

//     ASSERT_EQ(graph1.v_size(), 6);
//     ASSERT_EQ(graph2.v_size(), 6);
// }

// TEST(SingleWeightedGraphTest, MoveOperator) {
//     Graph<false, int, char, void> graph1 = { 
//         {1, {2, 3}},
//         {2, {4}   },
//         {3, {5, 4}},
//         {6, {1, 2, 3, 4, 5}}
//     };

//     Graph<false, int, char, void> graph2 = { 
//         {1, {2}   },
//         {2, {3}   },
//         {0, {1, 3}}
//     };

//     ASSERT_EQ(graph1.v_size(), 6);
//     ASSERT_EQ(graph2.v_size(), 4);
    
//     graph2 = std::move(graph1);

//     ASSERT_EQ(graph1.v_size(), 0);
//     ASSERT_EQ(graph2.v_size(), 6);
// }

// TEST(SingleWeightedGraphTest, AddVertexWithoutResource) {
//     Graph<true, int, std::string, void> graph = { 
//         {1, {0, 2, 3, 4}},
//         {2, {0, 1}   },
//         {3, {1, 4}},
//         {4, {1, 2, 3, 4, 0}}
//     };

//     ASSERT_EQ(graph.v_size(), 5);
    
//     try{
//         graph.add_vertex(1);
//         ASSERT_FALSE(true);
//     }
//     catch(tmn_exception::LogicException& e){
//         std::cout << e.what() << std::endl;
//     }

//     ASSERT_EQ(graph.v_size(), 5);

//     graph.add_vertex(5);
//     graph.add_vertex(6);
//     graph.add_vertex(7);

//     ASSERT_EQ(graph.v_size(), 8);
// }

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