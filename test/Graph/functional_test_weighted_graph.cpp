#include <gtest/gtest.h>
#include <string>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

using namespace tmn::graph;

TEST(WeightedGraphTest, DefaultConstructor) {
    Graph<true, char, char, complex_num> graph;
    ASSERT_EQ(graph.v_size(), 0);
}

TEST(WeightedGraphTest, ArraySeqConstructor) {
        
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

TEST(WeightedGraphTest, CopyConstructor) {
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

TEST(WeightedGraphTest, MoveConstructor) {
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

TEST(WeightedGraphTest, CopyOperator)  {
    
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

TEST(WeightedGraphTest, MoveOperator) {
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

TEST(WeightedGraphTest, AddVertexWithoutResource) {
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

    Graph<true, int, int, float> graph(seq);

    ASSERT_EQ(graph.v_size(), 5);
    
    graph.add_vertex(1);

    ASSERT_EQ(graph.v_size(), 5);

    graph.add_vertex(5);
    graph.add_vertex(6);
    graph.add_vertex(7);

    ASSERT_EQ(graph.v_size(), 8);
}

TEST(WeightedGraphTest, AddVertexWithResource) {
    Graph<true, int, std::string, complex_num> graph;
    graph.add_vertex(1, "string 1");
    graph.add_vertex(2, "string 2");
    graph.add_vertex(3, "string 3");
    graph.add_vertex(4, "string 4");
    graph.add_vertex(5, "string 5");
    ASSERT_EQ(graph.v_size(), 5);

    try {
        graph.add_vertex(3, "new string!");
        ASSERT_TRUE(false);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_FALSE(graph.add_vertex(3, "new string!", false));

    try {
        ASSERT_EQ(graph.get_resource(0), std::string("string 0"));
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_EQ(graph.get_resource(1), std::string("string 1"));
    ASSERT_EQ(graph.get_resource(2), std::string("string 2"));
    ASSERT_EQ(graph.get_resource(3), std::string("string 3"));
    ASSERT_EQ(graph.get_resource(4), std::string("string 4"));
    ASSERT_EQ(graph.get_resource(5), std::string("string 5"));
}

TEST(WeightedGraphTest, RemoveVertex) {
    Graph<true, int, std::string, complex_num> graph;
    graph.add_vertex(1, "string 1");
    graph.add_vertex(2, "string 2");
    graph.add_vertex(3, "string 3");
    graph.add_vertex(4, "string 4");
    graph.add_vertex(5, "string 5");

    try {
        graph.remove_vertex(10);
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_EQ(graph.v_size(), 5);
    ASSERT_FALSE(graph.remove_vertex(10, false));

    graph.remove_vertex(2);
    graph.remove_vertex(4);
    ASSERT_EQ(graph.v_size(), 3);

    ASSERT_EQ(graph.get_resource(1), std::string("string 1"));
    ASSERT_EQ(graph.get_resource(3), std::string("string 3"));
    ASSERT_EQ(graph.get_resource(5), std::string("string 5"));

    ASSERT_FALSE(graph.vertex_in_graph(2));
    ASSERT_FALSE(graph.vertex_in_graph(4));
    ASSERT_TRUE(graph.vertex_in_graph(1));
    ASSERT_TRUE(graph.vertex_in_graph(3));
    ASSERT_TRUE(graph.vertex_in_graph(5));

    try {
        graph.get_resource(2);
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    try {
        graph.get_resource(4);
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }
}

TEST(WeightedGraphTest, ConnectedVertices) {
    ArraySequence<tmn::Pair<tmn::Pair<int, int>, std::string>> seq = { 
        { {0, 2}, "light weight bby" }, 
        { {0, 4}, "false" },
        { {0, 6}, "hello world" },
        { {1, 3}, "char[]&*" }, 
        { {1, 5}, "dinner" }, 
        { {2, 1}, "face to face" },
        { {2, 3}, "qwerty" },
        { {2, 4}, "youtube" },
        { {2, 5}, "bash" },
        { {2, 6}, "filesystem" },
        { {3, 4}, "east" },
        { {4, 5}, "3543698744444444443945" },
        { {5, 1}, "string" },
        { {5, 2}, "int" },
        { {5, 3}, "float" },
        { {5, 4}, "smart" },
        { {5, 0}, "kolobok!" },
    };

    Graph<true, int, int, std::string> graph(seq);

    ASSERT_TRUE(graph.connected_vertices_count(0).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(1).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(2).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(3).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(4).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(5).has_value());
    ASSERT_TRUE(graph.connected_vertices_count(6).has_value());

    ASSERT_EQ(graph.connected_vertices_count(0).value(), 3);
    ASSERT_EQ(graph.connected_vertices_count(1).value(), 2);
    ASSERT_EQ(graph.connected_vertices_count(2).value(), 5);
    ASSERT_EQ(graph.connected_vertices_count(3).value(), 1);
    ASSERT_EQ(graph.connected_vertices_count(4).value(), 1);
    ASSERT_EQ(graph.connected_vertices_count(5).value(), 5);
    ASSERT_EQ(graph.connected_vertices_count(6).value(), 0);

    auto connected_v_0 = graph.connected_vertices(0);
    auto connected_v_2 = graph.connected_vertices(2);
    auto connected_v_3 = graph.connected_vertices(3);

    ASSERT_EQ(connected_v_0.size(), 3);
    ASSERT_TRUE(connected_v_0.contains(2) && connected_v_0.contains(4) && connected_v_0.contains(6));

    ASSERT_EQ(connected_v_2.size(), 5);
    ASSERT_TRUE(connected_v_2.contains(1) && connected_v_2.contains(3) && connected_v_2.contains(4) && connected_v_2.contains(5) && connected_v_2.contains(6));

    ASSERT_EQ(connected_v_3.size(), 1);
    ASSERT_TRUE(connected_v_3.contains(4));
}

TEST(WeightedGraphTest, ChangeVertexResource) {
    Graph<true, int, std::string, complex_num> graph;
    graph.add_vertex(0, "string 0");
    graph.add_vertex(1, "string 1");
    graph.add_vertex(2, "string 2");
    graph.add_vertex(3, "string 3");
    graph.add_vertex(4, "string 4");
    graph.add_vertex(5, "string 5");
    graph.add_vertex(6, "string 6");
    graph.add_vertex(7, "string 7");
    graph.add_vertex(8, "string 8");
    graph.add_vertex(9, "string 9");

    try{
        graph.change_vertex_resource(11, "string 11-11");
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    graph.change_vertex_resource(7, "string 777");
    graph.change_vertex_resource(7, "string 77");
    ASSERT_EQ(graph.get_resource(7), std::string("string 77"));
}

TEST(WeightedGraphTest, AllVertices) {
    ArraySequence<tmn::Pair<tmn::Pair<int, int>, std::string>> seq = { 
        { {0, 2}, "light weight bby" }, 
        { {0, 4}, "false" },
        { {0, 6}, "hello world" },
        { {1, 3}, "char[]&*" }, 
        { {1, 5}, "dinner" }, 
        { {2, 1}, "face to face" },
        { {2, 3}, "qwerty" },
        { {2, 4}, "youtube" },
        { {2, 5}, "bash" },
        { {2, 6}, "filesystem" },
        { {3, 4}, "east" },
        { {4, 5}, "3543698744444444443945" },
        { {5, 1}, "string" },
        { {5, 2}, "int" },
        { {5, 3}, "float" },
        { {5, 4}, "smart" },
        { {5, 0}, "kolobok!" },
    };

    Graph<true, int, int, std::string> graph(seq);

    auto set = graph.all_vertices();
    
    for (auto& elem : set){
        std::cout << elem << " ";
    } 
    std::cout << std::endl;
}

TEST(UnweightedGraphTest, AddEdge) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'Q', 'W'}, 8192 },
        { {'Q', 'R'}, 8192 },
        { {'Q', 'T'}, 8192 },
        { {'T', 'C'}, 8192 },
        { {'T', 'A'}, 8192 },
    };

    Graph<false, char, int, int> graph(seq);

    try {
        graph.add_edge('!', 'A');
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    try {
        graph.add_edge('A', '!');
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_FALSE(graph.add_edge('A', '!', false));
    ASSERT_FALSE(graph.add_edge('!', 'A', false));

    graph.add_edge('R', 'A');
    graph.add_edge('B', 'T', -1);
    graph.add_edge('A', 'Q', -1011010110);

    auto pair  = graph.weighted_adjacency_matrix();

    pretty_print_matrix(6, "FT SINGLEW-GRAPH", pair.first, pair.second);
    ASSERT_TRUE(is_symmetric_matrix(pair.first));
    ASSERT_TRUE(graph.is_connected('R', 'A'));
    ASSERT_TRUE(graph.is_connected('B', 'T'));
    ASSERT_TRUE(graph.is_connected('A', 'Q'));

    ASSERT_TRUE(graph.pass_weight('B', 'T').has_value());
    ASSERT_TRUE(graph.pass_weight('A', 'B').has_value());
    ASSERT_FALSE(graph.pass_weight('X', 'C').has_value());
    ASSERT_FALSE(graph.pass_weight('J', 'K').has_value());

    ASSERT_EQ(graph.pass_weight('B', 'C').value(), 2048);
    ASSERT_EQ(graph.pass_weight('A', 'Q').value(), -1011010110);
}

TEST(UnweightedGraphTest, RemoveEdge) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'Q', 'W'}, 8192 },
        { {'Q', 'R'}, 8192 },
        { {'Q', 'T'}, 8192 },
        { {'T', 'C'}, 8192 },
        { {'T', 'A'}, 8192 },
    };

    Graph<false, char, int, int> graph(seq);

    try {
        graph.remove_edge('!', 'A');
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    try {
        graph.remove_edge('A', '!');
        ASSERT_FALSE(true);
    }
    catch(tmn::exception::LogicException& e){
        std::cout << e.what() << std::endl;
    }

    ASSERT_FALSE(graph.remove_edge('A', '!', false));
    ASSERT_FALSE(graph.remove_edge('!', 'A', false));

    graph.remove_edge('Q', 'W');
    graph.remove_edge('Q', 'R');
    graph.remove_edge('Q', 'T');

    auto pair  = graph.basic_adjacency_list();

    pretty_print_matrix(7, "FT SINGLEW-GRAPH", pair.first, pair.second);
    ASSERT_TRUE(is_symmetric_matrix(pair.first));
    ASSERT_FALSE(graph.is_connected('Q', 'W'));
    ASSERT_FALSE(graph.is_connected('Q', 'R'));
    ASSERT_FALSE(graph.is_connected('Q', 'T'));
}

TEST(UnweightedGraphTest, FindPath) {
    tmn::sequence::ArraySequence<tmn::Pair<tmn::Pair<char, char>, int>> seq = { 
        { {'A', 'B'}, 256 }, 
        { {'A', 'C'}, 512 },
        { {'A', 'R'}, 22556 }, 
        { {'A', 'D'}, 1024 },
        { {'B', 'C'}, 2048 }, 
        { {'B', 'D'}, 4096 }, 
        { {'B', 'Q'}, 2048 }, 
        { {'Q', 'W'}, 8192 },
        { {'Q', 'R'}, 8192 },
        { {'Q', 'T'}, 8192 },
        { {'T', 'C'}, 8192 },
        { {'T', 'A'}, 8192 },
    };

    Graph<true, char, int, int> graph(seq);

    auto res = graph.find_shortest_path('A', 'R');  

    for (const auto& element : res){
        std::cout << element << " ";
    }
    std::cout << std::endl;   
}