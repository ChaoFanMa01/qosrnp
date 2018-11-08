#include <iostream>
#include <random>
#include <ctime>

#include "../src/header.h"
#include "../src/node.h"
#include "../src/graph.h"

std::uniform_real_distribution<double> d(-100.0, 100.0);
std::uniform_real_distribution<double> p(-10.0, 0.0);
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
            return new qosrnp::Sensor(qosrnp::Coordinate(d(e), d(e), d(e)), p(e), h(e), id++);
        case qosrnp::node_type::RELAY:
            return new qosrnp::Relay(qosrnp::Coordinate(d(e), d(e), d(e)), p(e), h(e), id++);
        case qosrnp::node_type::SINK:
            return new qosrnp::Sink(qosrnp::Coordinate(d(e), d(e), d(e)), p(e), h(e), id++);
    }
    return nullptr;
}

int main() {
    qosrnp::Nodes    nodes;

    for (int i = 0; i < 400; ++i)
        nodes.push_back(random_node());

    qosrnp::AdjacencyList<qosrnp::Node> al(nodes.begin(), nodes.end());

    std::cout << al;

    return 0;
}
