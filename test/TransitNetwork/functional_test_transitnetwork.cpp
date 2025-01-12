#include "gtest/gtest.h"

#include <random>

#include "../../include/Graph/TransitNetwork/TransitNetwork.hpp"

using namespace tmn::transit_network;

double cost_calc(const Route& route, TransportType type, bool first_entry) {
  double cost = route.base_cost;

  if(route.allowed_transport_types.contains(type)){
    if(route.allowed_transport_types[type].has_value()) {
      cost += route.distance * route.allowed_transport_types[type].value().enterance_cost;
    }
  }

  return cost;
}

unsigned int time_calc(const Route& route, TransportType transport_type) {
  unsigned int time = route.distance / (route.max_speed * (1 / route.traffic_level));

  return time;
}

TEST(TransitNetworkTest, FindOptimalPath) {
  tmn::Function<double (const Route&, TransportType, bool)> function_cost(cost_calc);
  tmn::Function<unsigned int (const Route&, TransportType)> function_time(time_calc);
  TransitNetwork network(function_cost, function_time);
  
  TransferPoint metro_station_kashirskaya(22, -45, "Kashirskaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_varshavskaya(14, -46, "Varshavskaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_kahovskaya(-2, -48, "Kahovskaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_sevastopolskaya(0, -44, "Sevastopolskaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_chertanovskaya(0, -49, "Chertanovskaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_yuzhnaya(0, -50, "Yuzhnaya", TransferPointType::METRO_STATION);
  TransferPoint metro_station_kantemirovkaya(16, -48, "Kantemirovkaya", TransferPointType::METRO_STATION);

  network.add_transfer_point(0, metro_station_kashirskaya);
  network.add_transfer_point(1, metro_station_varshavskaya);
  network.add_transfer_point(2, metro_station_kahovskaya);
  network.add_transfer_point(3, metro_station_sevastopolskaya);
  network.add_transfer_point(4, metro_station_chertanovskaya);
  network.add_transfer_point(5, metro_station_yuzhnaya);
  network.add_transfer_point(6, metro_station_kantemirovkaya);

  Route route1(2.33, 0.60, 65.4);
route1.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route2(2.21, 0.60, 61.4);
  route2.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route3(0.2, 0.00, 10);
  route3.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route4(2.78, 0.60, 63.8);
  route4.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route5(2.91, 0.60, 65.4);
  route5.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route6(2.1, 0.60, 62.6);
  route6.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});
  Route route7(3.12, 0.60, 66.9);
  route7.allowed_transport_types.insert({TransportType::METRO, tmn::Optional<Schedule>()});

  network.add_route(0, 1, route1); network.add_route(1, 0, route1);
  network.add_route(1, 2, route2); network.add_route(2, 1, route2);
  network.add_route(2, 3, route3); network.add_route(3, 2, route3);
  network.add_route(3, 4, route4); network.add_route(4, 3, route4);
  network.add_route(4, 5, route5); network.add_route(5, 4, route5);
  network.add_route(0, 6, route6); network.add_route(6, 0, route6);

  auto container = network.find_optimal_path(6, 3); 
  std::cout << container.size() << std::endl;

  for (const auto& item : container){
    std::cout << item << std::endl;
  }

}