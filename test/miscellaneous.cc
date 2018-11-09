#include <iostream>
#include <random>
#include <ctime>
#include <vector>

#include "../src/miscellaneous.h"

#define TEST_NUM    30

int main(void) {
    std::uniform_int_distribution<unsigned> d(0, 100);
    std::default_random_engine e(std::time(0));
    std::vector<int>    vec;

    for (int i = 0; i < TEST_NUM; ++i)
        vec.push_back(d(e));
    for (auto &e : vec)
        std::cout << e << " ";
    std::cout << std::endl;

    std::cout << "max: " << vec[qosrnp::index_of_max(vec.begin(), vec.end())] << ", ";
    std::cout << "min: " << vec[qosrnp::index_of_min(vec.begin(), vec.end())] << std::endl;
    std::cout << "after sorted: " << std::endl;
    qosrnp::insertion_sort_ascending(vec.begin(), vec.end());
    for (auto &e : vec)
        std::cout << e << " ";
    std::cout << std::endl;

    return 0;
}
