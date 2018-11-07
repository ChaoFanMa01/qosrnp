#include <iostream>
#include <random>
#include <ctime>

#include "../src/node.h"

int
main(void) {
    std::default_random_engine e(std::time(0));
    std::uniform_real_distribution<double> d(-100, 100);
    qosrnp::Nodes    nodes;

    for (int i = 0; i < 400; ++i)
        if (i < 10)
            nodes.push_back(new qosrnp::Sink(qosrnp::Coordinate(d(e), d(e), d(e)),
                            0.2, 10, i));
        else if (i < 100)
            nodes.push_back(new qosrnp::Sensor(qosrnp::Coordinate(d(e), d(e), d(e)),
                            0.2, 10, i));
        else
            nodes.push_back(new qosrnp::Relay(qosrnp::Coordinate(d(e), d(e), d(e)),
                            0.2, 10, i));

    for (int i = 0; i < 400; ++i)
        std::cout << *nodes[i] << std::endl;

    nodes.clear();

    return 0;
}
