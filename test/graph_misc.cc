#include <iostream>
#include <random>
#include <ctime>
#include <vector>

#include "../src/header.h"
#include "../src/graph_misc.h"
#include "../src/mysql_api.h"

std::uniform_real_distribution<double> d(0.0, 100.0);
std::uniform_real_distribution<double> p(0.0, 10.0);
std::uniform_int_distribution<unsigned> h(5, 20);
std::uniform_int_distribution<unsigned> u(0,2);
std::default_random_engine e(std::time(0));

qosrnp::id_type   id = 0;

qosrnp::node_type
random_node_type() {
    unsigned t = u(e);
    return t == 0 ? qosrnp::node_type::SENSOR :
           t == 1 ? qosrnp::node_type::RELAY : qosrnp::node_type::SINK;
}

qosrnp::Node *
random_node() {
    switch (random_node_type()) {
        case qosrnp::node_type::SENSOR:
            return new qosrnp::Sensor(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
        case qosrnp::node_type::RELAY:
            return new qosrnp::Relay(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
        case qosrnp::node_type::SINK:
            return new qosrnp::Sink(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
    }
    return nullptr;
}

qosrnp::Node *
random_node(qosrnp::node_type t) {
    switch (t) {
        case qosrnp::node_type::SENSOR:
            return new qosrnp::Sensor(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
        case qosrnp::node_type::RELAY:
            return new qosrnp::Relay(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
        case qosrnp::node_type::SINK:
            return new qosrnp::Sink(qosrnp::Coordinate(d(e), d(e), 0.0), p(e), h(e), id++);
    }
    return nullptr;
}

int main() {
    qosrnp::Nodes    nodes;
    char user[] = "root";
    char db[] = "cpp";
    char passwd[] = "0000000027";

    qosrnp::MySQLdb mysql(user, passwd, db);

    mysql.query("DELETE FROM graph");

    for (int i = 0; i < 400; ++i)
        if (i < 10)
            nodes.push_back(random_node(qosrnp::node_type::SINK));
        else if (i < 100)
            nodes.push_back(random_node(qosrnp::node_type::SENSOR));
        else
            nodes.push_back(random_node(qosrnp::node_type::RELAY));
    
    qosrnp::AdjacencyList<qosrnp::Node> al(nodes.begin(), nodes.end());

    if (!mysql.write_adjacency_list(qosrnp::dijkstra_spt(al, 0, std::vector<qosrnp::size_type>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10})))
        std::cout << "mysql error!" << std::endl;
    else
        std::cout << "mysql done." << std::endl;
    return 0;
}
