#include <random>
#include <iostream>
#include <ctime>
#include <vector>

#include "../src/header.h"
#include "../src/dc1np.h"
#include "../src/gqrnp.h"
#include "../src/mysql_api.h"
#include "../src/miscellaneous.h"

std::uniform_real_distribution<double> d(0.0, 100.0);
std::uniform_int_distribution<unsigned> u(0,2);
std::uniform_int_distribution<int> ui(0, 100);
std::uniform_int_distribution<unsigned> delta(10, 20);
std::default_random_engine e(std::time(0));
qosrnp::id_type id = 0;

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
    
    for (int i = 0; i < qosrnp::sink_num + qosrnp::sensor_num + qosrnp::relay_num; ++i) {
        if (i < qosrnp::sink_num) 
            nds.push_back(random_node(qosrnp::node_type::SINK));
        else if (i < qosrnp::sink_num + qosrnp::sensor_num)
            nds.push_back(random_node(qosrnp::node_type::SENSOR));
        else
            nds.push_back(random_node(qosrnp::node_type::RELAY));
    }
    
    {
    std::vector<qosrnp::Node*> nodes(nds.begin(), nds.end());
    std::set<qosrnp::size_type> y = qosrnp::dc1np(nodes);
    std::cout << "dc1np size: " << y.size() << std::endl;
    }

    for (auto &n : nds) {
        n->set_power(qosrnp::power);
        if (n->type() == qosrnp::node_type::SENSOR)
            n->set_hop(qosrnp::hop_constraint);
        else 
            n->set_hop(9999);
    }

/*    std::cout << "y_hat: ";
    for (auto &e : y)
        std::cout << e << ", ";
    std::cout <<  std::endl;*/
    
    std::vector<qosrnp::Node*> nodes(nds.begin(), nds.end());
/*    for (auto &n : nodes)
        std::cout << "hop: " << n->hop()
                  << ", power: " << n->power() << std::endl;
*/
    std::set<qosrnp::size_type> y = qosrnp::gqrnp(e, nodes);
    std::cout << "gqrnp size: " << y.size() << std::endl;

    return 0;
}

int main(void) {
    c1np_test();
    return 0;
}
