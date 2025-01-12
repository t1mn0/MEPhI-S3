#include "TransitNetwork.hpp"

#include <queue>

using namespace tmn::transit_network;

void TransitNetwork::add_transfer_point(unsigned int id, const TransferPoint& point) {
    try{
        add_vertex(id, point);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(add_transfer_point) : transit_network already contains transfer_point with such id: " + std::to_string(id));
    }
}

void TransitNetwork::add_transfer_point(unsigned int id, TransferPoint&& point) {
    try{
        add_vertex(id, point);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(add_transfer_point) : transit_network already contains transfer_point with such id: " + std::to_string(id));
    }
}

void TransitNetwork::remove_transfer_point(unsigned int id) {
    try{
        remove_vertex(id);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(remove_transfer_point) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
}

std::size_t TransitNetwork::size() const noexcept {
    return v_size();
}

const TransferPoint& TransitNetwork::get_transfer_point_info(unsigned int id) const {
    try{
        return get_resource(id);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(get_transfer_point_info) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
}

TransferPoint& TransitNetwork::get_transfer_point_info(unsigned int id) {
    try{
        return get_resource(id);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(get_transfer_point_info) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
}

void TransitNetwork::change_transfer_point_info(unsigned int id, const TransferPoint& point) {
    try{
        change_vertex_resource(id, point);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(change_transfer_point_info) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
}

void TransitNetwork::change_transfer_point_info(unsigned int id, TransferPoint&& point) {
    try{
        change_vertex_resource(id, point);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(change_transfer_point_info) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
}

void TransitNetwork::update_transfer_point_coordinates(unsigned int id, int x_coordinate, int y_coordinate) {
    if(!vertex_in_graph(id)){
        throw tmn::exception::LogicException("Error(update_transfer_point_coordinates) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
    resources[id].x_coordinate = x_coordinate;
    resources[id].y_coordinate = y_coordinate;
}

void TransitNetwork::update_transfer_point_type(unsigned int id, TransferPointType type) {
    if(!vertex_in_graph(id)){
        throw tmn::exception::LogicException("Error(update_transfer_point_type) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
    resources[id].type = type;
}

void TransitNetwork::update_transfer_point_parking_rates(unsigned int id, const HashTable<TransportType, double>& parking_rates) {
    if(!vertex_in_graph(id)){
        throw tmn::exception::LogicException("Error(update_transfer_point_parking_rates) : transfer_point with such id is not in the transit_network: " + std::to_string(id));
    }
    resources[id].parking_rates = parking_rates;
}

void TransitNetwork::add_route(unsigned int from, unsigned int to, const Route& route) {
    try{
        add_edge(from, to, route);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(add_route) -> (graph error): " + e.what());
    }
}

void TransitNetwork::add_route(unsigned int from, unsigned int to, Route&& route) {
    try{
        add_edge(from, to, route);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(add_route) -> (graph error): " + e.what());
    }
}

void TransitNetwork::remove_route(unsigned int from, unsigned int to) {
    try{
        remove_edge(from, to);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(remove_route) -> (graph error): " + e.what());
    }
}

bool TransitNetwork::has_route_between(unsigned int from, unsigned int to) const {
    try{
        return is_connected(from, to);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(has_route_between) -> (graph error): " + e.what());
    }
}

double TransitNetwork::passage_cost(unsigned int from, unsigned int to) const noexcept {
    return 0.0;
}

unsigned int TransitNetwork::passage_time(unsigned int from, unsigned int to) const noexcept {
    return 0;
}

void TransitNetwork::change_route(unsigned int from, unsigned int to, const Route& route) noexcept {
    try{
        change_weight(from, to, route);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(change_route) -> (graph error): " + e.what());
    }
}

void TransitNetwork::change_route(unsigned int from, unsigned int to, Route&& route) noexcept {
    try{
        change_weight(from, to, route);
    }
    catch(tmn::exception::LogicException& e){
        throw tmn::exception::LogicException("Error(change_route) -> (graph error): " + e.what());
    }
}

void TransitNetwork::update_route_distance(unsigned int from, unsigned int to, double distance) {
    if(!vertex_in_graph(from)){
        throw tmn::exception::LogicException("Error(update_route_distance) : transfer_point with such id is not in the transit_network: " + std::to_string(from));
    }
    if(!vertex_in_graph(to)){
        throw tmn::exception::LogicException("Error(update_route_distance) : transfer_point with such id is not in the transit_network: " + std::to_string(to));
    }
    if(!has_route_between(from, to)){
        throw tmn::exception::LogicException("Error(update_route_distance) : the specified transfer_point's are not connected in the transit_network");
    }
    adjacency_list[from][to].distance = distance;
}

void TransitNetwork::update_route_base_cost(unsigned int from, unsigned int to, double cost) {
    if(!vertex_in_graph(from)){
        throw tmn::exception::LogicException("Error(update_route_base_cost) : transfer_point with such id is not in the transit_network: " + std::to_string(from));
    }
    if(!vertex_in_graph(to)){
        throw tmn::exception::LogicException("Error(update_route_base_cost) : transfer_point with such id is not in the transit_network: " + std::to_string(to));
    }
    if(!has_route_between(from, to)){
        throw tmn::exception::LogicException("Error(update_route_base_cost) : the specified transfer_point's are not connected in the transit_network");
    }
    adjacency_list[from][to].base_cost = cost;
}

void TransitNetwork::update_route_max_speed(unsigned int from, unsigned int to, double max_speed) {
    if(!vertex_in_graph(from)){
        throw tmn::exception::LogicException("Error(update_route_max_speed) : transfer_point with such id is not in the transit_network: " + std::to_string(from));
    }
    if(!vertex_in_graph(to)){
        throw tmn::exception::LogicException("Error(update_route_max_speed) : transfer_point with such id is not in the transit_network: " + std::to_string(to));
    }
    if(!has_route_between(from, to)){
        throw tmn::exception::LogicException("Error(update_route_max_speed) : the specified transfer_point's are not connected in the transit_network");
    }
    adjacency_list[from][to].max_speed = max_speed;
}

void TransitNetwork::update_route_traffic_level(unsigned int from, unsigned int to, double traffic_level) {
    if(!vertex_in_graph(from)){
        throw tmn::exception::LogicException("Error(update_route_traffic_level) : transfer_point with such id is not in the transit_network: " + std::to_string(from));
    }
    if(!vertex_in_graph(to)){
        throw tmn::exception::LogicException("Error(update_route_traffic_level) : transfer_point with such id is not in the transit_network: " + std::to_string(to));
    }
    if(!has_route_between(from, to)){
        throw tmn::exception::LogicException("Error(update_route_traffic_level) : the specified transfer_point's are not connected in the transit_network");
    }
    adjacency_list[from][to].traffic_level = traffic_level;
}

ArraySequence<unsigned int> TransitNetwork::find_optimal_path(unsigned int start, unsigned int end, Optional<TransportType> transport_type) {
    ArraySequence<unsigned int> path;
    HashTable<unsigned int, PathInfo> min_path_info;

    if (!vertex_in_graph(start) || !vertex_in_graph(end)){
        return path;
    }

    for(const auto& pair : adjacency_list) {
        unsigned int vertex_id = pair.first;
        min_path_info.insert({vertex_id, PathInfo()});
    }

    min_path_info[start] = {0, 0, {start}};


    auto compare = [&](const Pair<unsigned int, PathInfo>& a, const Pair<unsigned int, PathInfo>& b) {
        return a.second < b.second;
    };

    std::priority_queue<Pair<unsigned int, PathInfo>, std::vector<Pair<unsigned int, PathInfo>>, decltype(compare)> unvisited_queue(compare);

    unvisited_queue.push({start, min_path_info[start]});

    while(!unvisited_queue.empty()) {
        unsigned int current_vertex = unvisited_queue.top().first;
        PathInfo current_path_info = unvisited_queue.top().second;
        unvisited_queue.pop();

        if(current_vertex == end) {
            path = current_path_info.path;
            return path;
        }

        if (adjacency_list.contains(current_vertex)){
            for (const auto& edge_pair : adjacency_list[current_vertex]) {
                unsigned int neighbor = edge_pair.first;
                const Route& route = edge_pair.second;
            
                if (transport_type.has_value()) {
                    TransportType type = transport_type.value();
                    double cost = cost_function(route, type, false);
                    unsigned int time = time_function(route, type);
                    double new_total_cost = current_path_info.total_cost + cost;
                    unsigned int new_total_time = current_path_info.total_time + time;

                    if(new_total_cost < min_path_info[neighbor].total_cost || new_total_time < min_path_info[neighbor].total_time) {
                        ArraySequence<unsigned int> new_path = current_path_info.path;
                        new_path.push_back(neighbor);
                        min_path_info[neighbor] = {new_total_cost, new_total_time, new_path};
                        unvisited_queue.push({neighbor, min_path_info[neighbor]});
                    }
                }
        
                for(int i = 0; i < 6; i++) {
                    TransportType transport_type = static_cast<TransportType>(i);
                    double cost = cost_function(route, transport_type, false);
                    unsigned int time = time_function(route, transport_type);

                    double new_total_cost = current_path_info.total_cost + cost;
                    unsigned int new_total_time = current_path_info.total_time + time;

                    if(new_total_cost < min_path_info[neighbor].total_cost || new_total_time < min_path_info[neighbor].total_time) {
                        ArraySequence<unsigned int> new_path = current_path_info.path;
                        new_path.push_back(neighbor);
                        min_path_info[neighbor] = {new_total_cost, new_total_time, new_path};
                        unvisited_queue.push({neighbor, min_path_info[neighbor]});
                    }
                }
            }
        }
    }

    return path;
}