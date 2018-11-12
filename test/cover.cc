#include <iostream>
#include <random>
#include <ctime>
#include <vector>

#include "../src/cover.h"

int main() {
    std::uniform_int_distribution<unsigned> u(0, 400);
    std::uniform_int_distribution<unsigned> s(10, 20);
    std::uniform_int_distribution<unsigned> ss(0, 20);
    std::uniform_int_distribution<unsigned> f(20, 50);
    std::default_random_engine e(std::time(0));
    qosrnp::Cover<int, int> cvr;
    std::vector<int>     vec;
    int tmp;

    
    for (int i = 0, n = s(e); i < n; ++i) {
        tmp = u(e);
        cvr.insert_set(tmp);
        vec.push_back(tmp);
    }

    std::uniform_int_distribution<unsigned> num(0, vec.size());
    for (int i = 0, n = f(e); i < n; ++i)
       for (int j = 0, k = ss(e); j < k; ++j)
           cvr.insert_family(i, vec[num(e)]);
    
    for (auto &c : cvr.family()) {
        std::cout << c.first << ": ";
        for (auto &h : c.second)
            std::cout << h << ", ";
        std::cout << std::endl;
    }
    std::cout << "set: ";
    for (auto &c : cvr.set())
        std::cout << c << ", ";
    std::cout << std::endl;
    
    std::cout << "min: ";
    for (auto &c : cvr.minimum_set_cover())
        std::cout << c << ",";
    std::cout << std::endl;
    std::cout << "k-cover: " << std::endl;
    for (auto &c : cvr.k_set_cover(3)) {
        std::cout << "set " << c << ": ";
        for (auto &e : cvr.family()[c])
            std::cout << e << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}

