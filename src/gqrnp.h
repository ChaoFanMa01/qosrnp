#ifndef QOSRNP_GQRNP_H
#define QOSRNP_GQRNP_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <random>
#include <cstdlib>

#include "header.h"
#include "node.h"
#include "graph.h"
#include "graph_misc.h"
#include "cover.h"
#include "rdc1np.h"

namespace qosrnp {
    // function predeclarations.
    void update_optimal(std::set<size_type>&, std::vector<std::set<size_type>>&);
    void calculate_fitness(std::vector<std::set<size_type>>&, std::vector<unsigned>&);
    size_type fitness(const std::set<size_type>&);
    size_type random_chromosome(std::default_random_engine&, std::vector<unsigned>&);
    void make_cross_poll(std::vector<Node*>&, const std::vector<Node*>&, 
                         std::set<size_type>, std::set<size_type>);

    /* @fn gqrnp()
     *
     * The Genetic-algorithm based QoS constrained Relay Node Placement 
     * (GQRNP) algorithm.
     */
    std::set<size_type>
    gqrnp(std::default_random_engine& en, const std::vector<Node*>& nds) {
        std::vector<std::set<size_type>>       population, mediate, current;
        std::vector<unsigned>                  roulette_wheel;
        std::set<size_type>                    tmp, optimal;

        // generate initial population.
        for (int i = 0; i < POPULATION; ) {
            try {
                std::vector<Node*>    nodes(nds.begin(), nds.end());
                tmp = rdc1np(en, nodes);
                std::cout << "building..." << std::endl;
            } catch (std::range_error e) {
                continue;
            }
            if (!tmp.empty()) {
                std::cout << "tmp size: " << tmp.size() << std::endl;
                population.push_back(tmp);
                ++i;
            }
            tmp.clear();
        }

        update_optimal(optimal, population);
        std::cout << "current optimal: " << optimal.size() << std::endl;

        // reproduce
        for (int i = 0; i < GENERATION; ++i) {
            // calculate the fitness of each chromosome, and build
            // the roulette wheel based on the fitness calculation.
            calculate_fitness(population, roulette_wheel);
            // selection process.
            // select POPULATION chromosomes from the last generation
            // using the random roulette wheel method.
            for (int j = 0; j < POPULATION; ++j)
                mediate.push_back(population[random_chromosome(en, roulette_wheel)]);
            
            // crossover process.
            for (int j = 0; j < POPULATION; j += 2) {
                std::vector<Node*>      cross_poll;
                std::cout << "mediate size: " << mediate.size() << ", j=" << j << std::endl;
                make_cross_poll(cross_poll, nds, mediate[j], mediate[j + 1]);
                for (int k = 0; k < 2; ) {
                    try {
                        std::cout << "trying" << std::endl;
                        tmp = rdc1np(en, cross_poll);
                    } catch (std::range_error e) {
                        continue;
                    }
                    if (!tmp.empty()) {
                        std::cout << "trying over" << std::endl;
                        current.push_back(tmp);
                        ++k;
                    }
                    tmp.clear();
                }
            }
            roulette_wheel.clear();
            population.clear();
            mediate.clear();
            population = current;
            // update optimal solution.
            update_optimal(optimal, population);
            std::cout << "current optimal: " << optimal.size() << std::endl;
        }
        return optimal;
    }

    void
    update_optimal(std::set<size_type>& op, std::vector<std::set<size_type>>& popu) {
        std::cout << "update optimal" << std::endl;
        for (auto &ch : popu)
            if (op.size() == 0)
                op = ch;
            else if (ch.size() < op.size()) {
                op.clear();
                op = ch;
            }
    }

    void
    calculate_fitness(std::vector<std::set<size_type>>& po, std::vector<unsigned>& rw) {
        std::cout << "calculate fitness" << std::endl;
        for (int i = 0; i < po.size(); ++i)
            if (i == 0)
                rw.push_back(fitness(po[i]));
            else
                rw.push_back(rw[i - 1] + fitness(po[i]));
    }

    size_type
    fitness(const std::set<size_type>& ch) {
        return CDL_NUM - ch.size();
    }

    size_type
    random_chromosome(std::default_random_engine& e, std::vector<unsigned>& rw) {
        std::uniform_int_distribution<unsigned>  u(0, rw.back());
        unsigned                                 r = u(e);
        std::cout << "random chromosome" << std::endl;

        for (int i = 0; i < rw.size(); ++i)
            if (r <= rw[i])
                return i;
    }

    void
    make_cross_poll(std::vector<Node*>& cp, const std::vector<Node*>& total, 
                    std::set<size_type> p1, std::set<size_type> p2) {
        std::set<size_type>    pp;
        std::cout << "make cross poll" << std::endl;

        std::cout << "poll over" << std::endl;

        for (auto e : p1)
            pp.insert(e);
        for (auto e : p2)
            pp.insert(e);

        std::cout << "poll over" << std::endl;
        for (auto &n : total)
            if (n->type() != node_type::RELAY)
                cp.push_back(n);
            else
                break;

        std::cout << "poll over" << std::endl;
        for (auto &e : pp)
            cp.push_back(total[e]);
        std::cout << "poll over" << std::endl;
    }
}
#endif
