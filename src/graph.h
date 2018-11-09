#ifndef QOSRNP_GRAPH_H
#define QOSRNP_GRAPH_H

#include <iostream>
#include <utility>
#include <cstdint>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <climits>      // INT_MAX
#include <cfloat>       // DBL_MAX

#include "header.h"
#include "node.h"

namespace qosrnp {
    // type declarations.
    template <class C> class Vertex;
    template <class C> class Edge;
    template <class C> class AdjacencyList;

    // function declarations.
    template <class C>
    std::ostream& operator<<(std::ostream&, const Edge<C>&);
    template <class C>
    std::ostream& operator<<(std::ostream&, const Vertex<C>&);
    template <class C>
    std::ostream& operator<<(std::ostream&, const AdjacencyList<C>&);


    /* @class Edge
     *
     * Class representing an edge in an undirected graph.
     */
    template <class C>
    class Edge {
    public:
        typedef C            node_type;
        typedef Vertex<C>    vertex_type;
        typedef double       weight_type;

        static const weight_type   DEFAULT_WEIGHT;

        Edge(vertex_type* h = nullptr,
             vertex_type* t = nullptr,
             const weight_type& w = DEFAULT_WEIGHT)
        : _head(h), _tail(t), _weight(w) {}

        Edge(const Edge&);
        Edge(Edge&&);
        ~Edge() { _head = _tail = nullptr;}

        Edge& operator=(const Edge&);
        Edge& operator=(Edge&&);
        bool  operator==(const Edge&) const;
        bool  operator!=(const Edge&) const;

        bool operator>(const Edge&) const = delete;
        bool operator>=(const Edge&) const = delete;
        bool operator<(const Edge&) const = delete;
        bool operator<=(const Edge&) const = delete;

        vertex_type*  head() const { return _head; }
        vertex_type*  tail() const { return _tail; }
        weight_type   weight() const { return _weight; }

        void set_head(const vertex_type* h) { _head = h; }
        void set_tail(const vertex_type* t) { _tail = t; }
        void set_weight(const weight_type& w) { _weight = w; }

    private:
        vertex_type*         _head;
        vertex_type*         _tail;
        weight_type          _weight;
    };

    template <class C>
    const typename Edge<C>::weight_type Edge<C>::DEFAULT_WEIGHT = -1.0;

    template <class C>
    Edge<C>::Edge(const Edge& e)
    : _head(e._head), _tail(e._tail), _weight(e._weight) {}

    template <class C>
    Edge<C>::Edge(Edge&& e)
    : _head(std::move(e._head)), _tail(std::move(e._tail)),
      _weight(std::move(e._weight)) {}

    template <class C>
    Edge<C>&
    Edge<C>::operator=(const Edge& e) {
        _head = e._head;
        _tail = e._tail;
        _weight = e._weight;
        return *this;
    }

    template <class C>
    Edge<C>&
    Edge<C>::operator=(Edge&& e) {
        _head = std::move(e._head);
        _tail = std::move(e._tail);
        _weight = std::move(e._weight);

        e._head = e._tail = nullptr;

        return *this;
    }

    template <class C>
    bool
    Edge<C>::operator==(const Edge& e) const {
        return _head == e._head && _tail == e._tail &&
               _weight == e._weight;
    }

    template <class C>
    bool
    Edge<C>::operator!=(const Edge& e) const {
        return !(*this == e);
    }


    /* @fn Vertex
     *
     * Class representing a vertex in an undirected graph.
     */
    template <class C>
    class Vertex {
    public:
        typedef C                                         node_type;
        typedef int32_t                                   weight_type;
        typedef typename std::vector<Edge<C>>::size_type  size_type;

        const static id_type       DEFAULT_ID     = -1;
        const static id_type       DEFAULT_PARENT;
        const static weight_type   DEFAULT_WEIGHT;

        Vertex(node_type* n = nullptr, 
               const id_type& i = DEFAULT_ID,
               const weight_type& w = DEFAULT_WEIGHT,
               const id_type& p = DEFAULT_PARENT,
               const std::vector<Edge<C>>& neis = std::vector<Edge<C>>())
        : _node(n), _id(i), _weight(w), _parent(p), _neighbors(neis){}

        Vertex(const Vertex&);
        Vertex(Vertex&&);
        ~Vertex() { _node = nullptr; }

        Vertex& operator=(const Vertex&);
        Vertex& operator=(Vertex&&);
        bool operator==(const Vertex& v) const { return _id == v._id; }
        bool operator!=(const Vertex& v) const { return !(*this == v); }
        bool operator>(const Vertex& v) const { return _weight > v._weight; }
        bool operator>=(const Vertex& v) const { return _weight >= v._weight; }
        bool operator<(const Vertex& v) const { return _weight < v._weight; }
        bool operator<=(const Vertex& v) const { return _weight <= v._weight; }

        node_type*       node() const { return _node; }
        id_type          id() const { return _id; }
        weight_type      weight() const { return _weight; }
        id_type          parent() const { return _parent; }

        void   set_node(const node_type* n) { _node = n; }
        void   set_id(const id_type& i) { _id = i; }
        void   set_weight(const weight_type& w) { _weight = w; }
        void   set_parent(const id_type& p) { _parent = p; }

        std::vector<Edge<C>>  neighbors() const { return _neighbors; }
        std::vector<Edge<C>>& neighbors() { return _neighbors; }
        void push_neighbor(const Edge<C>& e) { _neighbors.push_back(e); }
        void pop_neighbor() { _neighbors.pop_back(); }
        void clear_neighbor() { _neighbors.clear(); }
        size_type size_neighbor() const { return _neighbors.size(); }

    private:
        node_type*             _node;
        id_type                _id;
        weight_type            _weight;
        id_type                _parent;
        std::vector<Edge<C>>   _neighbors;
    };

    template <class C>
    const id_type Vertex<C>::DEFAULT_PARENT = -1;
    
    template <class C>
    const typename Vertex<C>::weight_type Vertex<C>::DEFAULT_WEIGHT = 9999;

    template <class C>
    Vertex<C>::Vertex(const Vertex& v)
    : _node(v._node), _id(v._id), _weight(v._weight),
      _parent(v._parent), _neighbors(v._neighbors) {}

    template <class C>
    Vertex<C>::Vertex(Vertex&& v) 
    : _node(std::move(v._node)), _id(std::move(v._id)),
      _weight(std::move(v._weight)), _parent(std::move(v._parent)),
      _neighbors(std::move(v._neighbors)) {
        v._node = nullptr;
    }

    template <class C>
    Vertex<C>&
    Vertex<C>::operator=(const Vertex& v) {
        _node = v._node; _id = v._id;
        _weight = v._weight;
        _parent = v._parent;
        _neighbors = v._neighbors;

        return *this;
    }

    template <class C>
    Vertex<C>&
    Vertex<C>::operator=(Vertex&& v) {
        _node = std::move(v._node);
        _id = std::move(v._id);
        _weight = std::move(v._weight);
        _parent = std::move(v._parent);
        _neighbors = std::move(v._neighbors);

        v._node = nullptr;

        return *this;
    }
 
    /* @class AdjacencyList
     *
     * Adjacency list of an undirected graph.
     */
    template <class C>
    class AdjacencyList {
    public:
        typedef C                                               node_type;
        typedef typename Edge<C>::weight_type                   weight_type;
        typedef typename std::vector<Vertex<C>>::size_type      size_type; 
        typedef typename std::vector<Vertex<C>>::iterator       iterator;
        typedef typename std::vector<Vertex<C>>::const_iterator const_iterator;

        static const weight_type     INFTY;

        AdjacencyList() : vertices(std::vector<Vertex<C>>()) {}
        AdjacencyList(const AdjacencyList& al)
        : vertices(al.vertices) {}
        AdjacencyList(AdjacencyList&& al)
        : vertices(std::move(al.vertices)) {}

        template <class Itr> AdjacencyList(Itr, Itr);
        
        ~AdjacencyList() = default;

        AdjacencyList& operator=(const AdjacencyList&);
        AdjacencyList& operator=(AdjacencyList&&);

        bool operator==(const AdjacencyList&) const = delete;
        bool operator!=(const AdjacencyList&) const = delete;
        bool operator>(const AdjacencyList&) const = delete;
        bool operator>=(const AdjacencyList&) const = delete;
        bool operator<(const AdjacencyList&) const = delete;
        bool operator<=(const AdjacencyList&) const = delete;

        Vertex<C>& operator[](size_type i) { return vertices[i]; }
        const Vertex<C>& operator[](size_type i) const { return vertices[i]; }

        iterator       begin() noexcept { return vertices.begin(); }
        const_iterator begin() const noexcept { return vertices.begin(); }
        iterator       end() noexcept { return vertices.end(); }
        const_iterator end() const noexcept { return vertices.end(); }

        void push_back(const Vertex<C>& v) { vertices.push_back(v); }
        void clear() { vertices.clear(); }

        size_type size() const { return vertices.size(); }
    private:
        std::vector<Vertex<C>>    vertices;
    };

    template <class C>
    const typename AdjacencyList<C>::weight_type
    AdjacencyList<C>::INFTY = DBL_MAX;

    template <class C>
    template <class Itr>
    AdjacencyList<C>::AdjacencyList(Itr b, Itr e)
    : vertices(std::vector<Vertex<C>>()) {
        // add vertices.
        for (Itr itr = b; itr != e; ++itr)
            vertices.push_back(Vertex<C>(*itr, vertices.size()));
        // add edges for each vertex.
        for (size_type i = 0; i < vertices.size(); ++i)
            for (size_type j = 0; j < vertices.size(); ++j)
                if (i != j && is_neighbor(*vertices[i].node(), 
                                          *vertices[j].node())) {
                    vertices[i].push_neighbor(Edge<C>(&vertices[i],
                                                      &vertices[j]));
                }
    }

    template <class C>
    AdjacencyList<C>&
    AdjacencyList<C>::operator=(const AdjacencyList& al) {
        vertices = al.vertices;
        return *this;
    }

    template <class C>
    AdjacencyList<C>&
    AdjacencyList<C>::operator=(AdjacencyList&& al) {
        vertices = std::move(al.vertices);
        return *this;
    }

    template <class C>
    std::ostream&
    operator<<(std::ostream& os, const Edge<C>& e) {
        os << "[" << e.head()->id() << "-->" << e.tail()->id()
           << " w: " << e.weight() << "]";
        return os;
    }

    template <class C>
    std::ostream& 
    operator<<(std::ostream& os, const Vertex<C>& v) {
        os << "vertex: " << *v.node() << std::endl;
        std::vector<Edge<C>> neighbors = v.neighbors();
        if (neighbors.size() != 0) {
            os << "edges: ";
            for (typename Vertex<C>::size_type i = 0; i < neighbors.size(); ++i)
                os << neighbors[i] << " ";
            os << std::endl;
        } else {
            os << "isolated" << std::endl;
        }
        return os;
    }


    template <class C>
    std::ostream&
    operator<<(std::ostream& os, const AdjacencyList<C>& al) {
        for (typename AdjacencyList<C>::size_type i = 0; i < al.size(); ++i)
            os << al[i] << std::endl;
        return os;
    }
}

#endif
