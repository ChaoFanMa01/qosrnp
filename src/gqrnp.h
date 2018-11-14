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
#include "dc1np.h"
#include "rdc1np.h"

namespace qosrnp {
    // function predeclarations.
    void update_optimal(std::set<size_type>&, std::vector<std::set<size_type>>&);
    void calculate_fitness(std::vector<std::set<size_type>>&, std::vector<unsigned>&);
    size_type fitness(const std::set<size_type>&);
    size_type random_chromosome(std::default_random_engine&, std::vector<unsigned>&);
    void make_cross_poll(std::vector<Node*>&, const std::set<size_type>&, 
                         const std::set<size_type>&);
    bool is_in_set(const size_type&, const std::set<size_type>&);

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
                for (auto &n : nds) {
                    n->set_power(qosrnp::power);
                    if (n->type() == qosrnp::node_type::SENSOR)
                        n->set_hop(qosrnp::hop_constraint);
                    else 
                        n->set_hop(9999);
                }
                std::vector<Node*>    nodes(nds.begin(), nds.end());
                if (i == 0)
                    tmp = dc1np(nodes);
                else
                    tmp = rdc1np(en, nodes);
            } catch (std::range_error e) {
                continue;
            }
            if (!tmp.empty()) {
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
                for (int k = 0; k < 2; ) {
                    try {
                        for (auto &n : nds) {
                            n->set_power(qosrnp::power);
                            if (n->type() == qosrnp::node_type::SENSOR)
                                n->set_hop(qosrnp::hop_constraint);
                            else 
                                n->set_hop(9999);
                        }
                        std::vector<Node*>      cross_poll(nds.begin(), nds.end());
                        make_cross_poll(cross_poll, mediate[j], mediate[j + 1]);
                        if (k == 0)
                            tmp = dc1np(cross_poll);
                        else
                            tmp = rdc1np(en, cross_poll);
                    } catch (std::range_error e) {
                        continue;
                    }
                    if (!tmp.empty()) {
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

        for (int i = 0; i < rw.size(); ++i)
            if (r <= rw[i])
                return i;
    }

    void
    make_cross_poll(std::vector<Node*>& cp, const std::set<size_type>& p1, 
                    const std::set<size_type>& p2) {
        std::set<size_type>    pp;

        for (auto &n : cp)
            if ((n->type() != node_type::SINK) && 
                (n->type() != node_type::SENSOR) &&
                !(is_in_set(n->id(), p1) || is_in_set(n->id(), p2))) {
                n->set_power(0.0);
            }
    }

    bool
    is_in_set(const size_type& n, const std::set<size_type>& s) {
        for (auto &e : s)
            if (n == e)
                return true;
        return false;
    }
}
#endif
