#ifndef QOSRNP_GRAPH_MISC_H
#define QOSRNP_GRAPH_MISC_H

#include <vector>

#include "header.h"
#include "node.h"
#include "graph.h"
#include "miscellaneous.h"

namespace qosrnp {
    // function predeclarations.
    template <class C>
    bool is_in(const std::vector<Vertex<C>>&, const Vertex<C>&);

    template <class C>
    bool breadth_first_traverse(const AdjacencyList<C>&, bool);

    template <class C>
    AdjacencyList<C> breadth_first_tree(const AdjacencyList<C>&);

    template <class C>
    bool is_connected(const AdjacencyList<C>&, size_type, const std::vector<size_type>&);

    template <class C>
    bool has_edge(const Edge<C>&, const std::vector<Edge<C>>&);
    
    hop_type max_hop(const AdjacencyList<Node>&, 
                     const std::vector<size_type>&);

    bool meet_hop(const AdjacencyList<Node>&, const size_type&, 
                  const std::vector<size_type>&);

    /* @fn breadth_first_traverse()
     *
     * Traverse given graph using the breadth first algorithm,
     * meanwhile checking the connectivity of this graph.
     * @param p if true, print the traverse information.
     * @return true if this graph is connected, false if 
     * disconnected.
     */
    template <class C>
    bool
    breadth_first_traverse(const AdjacencyList<C>& al, bool p) {
        std::vector<Vertex<C>> grey, temp_grey, black;

        grey.push_back(al[0]);

        while (!grey.empty()) {
            while (!grey.empty()) {
                int cnt = 0;
                black.push_back(grey.back());
                for (auto &e : grey.back().neighbors()) {
                    if (!is_in(black, *e.tail()) && !is_in(grey, *e.tail()) && !is_in(temp_grey, *e.tail())) {
                        ++cnt;
                        temp_grey.push_back(*e.tail());
                        if (p)
                            std::cout << "v" << grey.back().id()
                                      << "->" << "v" << e.tail()->id()
                                      << "\t";
                    }
                }
                if (cnt && p)
                    std::cout << std::endl;
                grey.pop_back();
            }
            grey = temp_grey;
            temp_grey.clear();
        }
        if (black.size() < al.size())
            return false;
        return true;
    }

    /* @fn breadth_first_tree()
     *
     * Build a breadth first traverse tree on given graph.
     */
    template <class C>
    AdjacencyList<C>
    breadth_first_tree(const AdjacencyList<C>& graph) {
        std::vector<Vertex<C>> grey, temp_grey, black;
        AdjacencyList<C> al;

        for (auto &v : graph)
            al.push_back(Vertex<C>(v.node(), al.size()));

        grey.push_back(graph[0]);

        while (!grey.empty()) {
            while (!grey.empty()) {
                black.push_back(grey.back());
                for (auto &e : grey.back().neighbors()) {
                    if (!is_in(black, *e.tail()) && !is_in(grey, *e.tail()) &&
                        !is_in(temp_grey, *e.tail())) {
                        temp_grey.push_back(*e.tail());
                        al[black.back().id()].push_neighbor(Edge<C>(&al[black.back().id()], &al[e.tail()->id()]));
                    }
                }
                grey.pop_back();
            }
            grey = temp_grey;
            temp_grey.clear();
        }
        return al;
    }

    template <class C>
    bool
    is_in(const std::vector<Vertex<C>>& vec, const Vertex<C>& v) {
        for (auto &vv : vec)
            if (vv == v)
                return true;
        return false;
    }

    template <class C>
    bool
    is_connected(const AdjacencyList<C>& al,
                 size_type src,
                 const std::vector<size_type>& dests) {
        std::vector<Vertex<C>> grey, temp_grey, black;
        int cnt = 0;
        bool jump = false;

        grey.push_back(al[src]);

        while (!grey.empty() && !jump) {
            while (!grey.empty() && !jump) {
                black.push_back(grey.back());
                for (auto &e : grey.back().neighbors()) {
                    if (!is_in(black, *e.tail()) && !is_in(grey, *e.tail()) &&
                        !is_in(temp_grey, *e.tail())) {
                        temp_grey.push_back(*e.tail());
                        if (index_of(dests.begin(), dests.end(), 
                            e.tail()->id()) != -1) {
                            if (++cnt == dests.size()) {
                                jump = true; break;
                            }
                        }
                    }
                }
                grey.pop_back();
            }
            grey = temp_grey;
            temp_grey.clear();
        }
        return jump;
    }

    template <class C>
    bool
    has_edge(const Edge<C>& e, const std::vector<Edge<C>>& es) {
        for (auto &ee : es)
            if (ee == e)
                return true;
        return false;
    }

    template <class C>
    AdjacencyList<C>
    dijkstra_spt(AdjacencyList<C>& graph, size_type src,
                 std::vector<size_type> dests) {
        std::vector<Vertex<C>> grey, black;
        AdjacencyList<C> al, spt;

        if (src < 0 || src >= graph.size()) {
#if !defined(NDEBUG)
            std::cerr << "function" << __func__
                      << "in file"  << __FILE__
                      << "at line"  << __LINE__
                      << ": No such vertex in this graph!"
                      << std::endl;
            std::exit(-1);
#else
            throw std::range_error("No such vertex in this graph!");
#endif
        }

        for (auto &d : dests)
            if (d < 0 || d >= graph.size() || d == src)
                throw std::range_error("No such vertex in this graph!");

        if (!is_connected(graph, src, dests))
            throw std::range_error("Source cannot connect all destinations.");

        for (auto &v : graph) {
            al.push_back(Vertex<C>(v.node(), al.size()));
            spt.push_back(Vertex<C>(v.node(), al.size()));
        }

        al[src].set_weight(0);
        grey.push_back(al[src]);

        while (!grey.empty()) {
            ssize_t index;
            insertion_sort_descending(grey.begin(), grey.end());
            Vertex<C> min = grey.back();
            black.push_back(min);
            grey.pop_back();
            // update shortest distance to the source node and search 
            // new grey nodes.
            for (auto &e : graph[min.id()].neighbors()) {
                // a node having a distance greater than the minimal 
                // grey node must be a grey or white node.
                if (al[e.tail()->id()].weight() > al[min.id()].weight() + 1) {
                    al[e.tail()->id()].set_weight(al[min.id()].weight() + 1);
                    al[e.tail()->id()].set_parent(min.id());
                    // if this neighbor is a grey node, update its distance.
                    if ((index = index_of(grey.begin(), grey.end(), *e.tail())) >= 0) {
                        grey[index].set_weight(al[min.id()].weight() + 1);
                    // if this neighbor is a white node, make it grey.
                    } else if (index_of(black.begin(), black.end(), *e.tail()) == -1) {
                        grey.push_back(al[e.tail()->id()]);
                    // if this neighbor is a black node, error occurs.
                    } else {
                        std::cerr << "Algorithm run into wrong state." << std::endl;
                        throw std::range_error("SPT updating black node");
                    }
                }
            }
            // adding edge between min node and its parent.
            // source node has no parent.
            if (min.id() != src) {
//                 al[min.id()].push_neighbor(edge<N>(&al[min.id()], &al[min.parent()]));
                 al[min.parent()].push_neighbor(Edge<C>(&al[min.parent()], &al[min.id()]));
            }
        }
        
        for (size_type i = 0; i < al.size(); ++i)
            graph[i].set_weight(al[i].weight());
        // the leaves of this newly built shortest path tree may not be given
        // destinations. So, we now create a shortest path tree whose leaves
        // are only given destinations according to the newly built shortest
        // path tree.
        for (size_type i = 0; i < dests.size(); ++i) {
            for (id_type j = al[dests[i]].id(); j != src; j = al[j].parent()) {
                spt[j].set_parent(al[j].parent());
                if (has_edge(Edge<C>(&spt[spt[j].parent()], &spt[j]), 
                             spt[spt[j].parent()].neighbors())) break;
                spt[spt[j].parent()].push_neighbor(Edge<C>(&spt[spt[j].parent()], &spt[j]));
            }
        }
        return spt;
    }
    
    /* @fn max_hop
     * Find the maximum delta among given destinations from a graph.
     */
    hop_type max_hop(const AdjacencyList<Node>& res, 
                         const std::vector<size_type>& dests) {
        hop_type max = 0;
        for (auto &d : dests)
            if (max < res[d].node()->hop())
                max = res[d].node()->hop();
        return max;
    }

    /* @fn meet_hop
     * Check whether all destinations on a given graph
     * meet their hop constraints.
     * @return true if meet, false otherwise.
     */
    bool meet_hop(const AdjacencyList<Node>& al, 
                    const size_type& src, 
                    const std::vector<size_type>& dests) {
        for (auto &i : dests)
            if (al[i].weight() > al[i].node()->hop())
                return false;
          return true;
    }
}

#endif
