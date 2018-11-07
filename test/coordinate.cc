#include <iostream>
#include <random>
#include <ctime>       // time(0)

#include "../src/coordinate.h"

#define TEST_NUM      100

int
main(void) {
    std::uniform_real_distribution<double>    d(-100, 100);
    std::default_random_engine e(std::time(0));

/*
    for (int i = 0; i < TEST_NUM; ++i) {
        qosrnp::Coordinate  n1(d(e), d(e), d(e));
        qosrnp::Coordinate  n2(d(e), d(e), d(e));

        std::cout << "Coordinate 1: " << n1;
        std::cout << "Coordinate 2: " << n2 << std::endl;

        std::cout << (n1 == n2 ? "identical" : "different");
        std::cout << ", distance: " << qosrnp::distance(n1, n2) << std::endl;
    }*/
    qosrnp::Coordinate n1(10.0, 10.0, 10.01), n2(10.0, 10.0, 10.0);
    std::cout << (n1 == n2 ? "identical" : "different") << std::endl;

    return 0;
}
