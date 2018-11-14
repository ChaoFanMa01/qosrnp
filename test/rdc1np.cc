#include <random>
#include <iostream>
#include <ctime>
#include <vector>

#include "../src/header.h"
#include "../src/dc1np.h"
#include "../src/rdc1np.h"
#include "../src/mysql_api.h"
#include "../src/miscellaneous.h"

std::uniform_real_distribution<double> d(0.0, 100.0);
std::uniform_int_distribution<unsigned> u(0,2);
std::uniform_int_distribution<int> ui(0, 100);
std::uniform_int_distribution<unsigned> delta(10, 20);
std::default_random_engine e(std::time(0));
qosrnp::id_type id = 0;

bool
is_contain(qosrnp::size_type id, std::set<qosrnp::size_type>& s) {
    for (auto &e : s)
        if (e == id)
            return true;
    return false;
}

qosrnp::Node*
random_node(qosrnp::node_type t) {
    switch(t) {
    case qosrnp::node_type::SENSOR:
        return new qosrnp::Sensor(qosrnp::Coordinate(d(e), d(e), 0.0), 10.0, 15, id++);
    case qosrnp::node_type::RELAY:
        return new qosrnp::Relay(qosrnp::Coordinate(d(e), d(e), 0.0), 10.0, 9999, id++);
    case qosrnp::node_type::SINK:
        return new qosrnp::Sink(qosrnp::Coordinate(d(e), d(e), 0.0), 10.0, 9999, id++);
    }
    return nullptr;
}

int c1np_test(void) {
    qosrnp::Nodes nds;
    char user[] = "root";
    char db[] = "cpp";
    char passwd[] = "0000000027";

    qosrnp::MySQLdb mysql(user, passwd, db);

    mysql.query("DELETE FROM graph");
    
    for (int i = 0; i < 400; ++i) {
        if (i < 1) 
            nds.push_back(random_node(qosrnp::node_type::SINK));
        else if (1 <= i && i < 40)
            nds.push_back(random_node(qosrnp::node_type::SENSOR));
        else
            nds.push_back(random_node(qosrnp::node_type::RELAY));
    }
    {std::vector<qosrnp::Node*> nodes(nds.begin(), nds.end());
    std::set<qosrnp::size_type> y = qosrnp::dc1np(nodes);
/*    std::cout << "y_hat: ";
    for (auto &e : y)
        std::cout << e << ", ";
    std::cout <<  std::endl;*/
    std::cout << "dc1np size: " << y.size() << std::endl;}
    
//    for (int i = 0; i < 50; ++i) {
        std::vector<qosrnp::Node*> nodes(nds.begin(), nds.end());
        std::set<qosrnp::size_type> y = qosrnp::rdc1np(e, nodes);
/*        std::cout << "y_hat: ";
        for (auto &e : y)
            std::cout << e << ", ";
        std::cout <<  std::endl;*/
        std::cout << "rdc1np size: " << y.size() << std::endl;
//    }
    std::vector<qosrnp::Node*> res(nds.begin(), nds.end());

    for (auto &n : res)
        if (n->type() != qosrnp::node_type::SINK &&
            n->type() != qosrnp::node_type::SENSOR &&
            !is_contain(n->id(), y))
            n->set_power(0.0);
     qosrnp::AdjacencyList<qosrnp::Node> al(res.begin(), res.end());
     std::vector<qosrnp::size_type> dests;
     for (auto &n : res)
         if(n->type() == qosrnp::node_type::SENSOR)
             dests.push_back(n->id());
     if (!mysql.write_adjacency_list(qosrnp::dijkstra_spt(al, 0, dests)))
         std::cout << "mysql error!" << std::endl;
     else
         std::cout << "mysql done." << std::endl;

    return 0;
}


int main(void) {
    c1np_test();
    return 0;
}
