#ifndef QOSRNP_NODE_H
#define QOSRNP_NODE_H

#include <iostream>
#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "header.h"
#include "coordinate.h"

namespace qosrnp {
    // class predeclarations.
    enum class node_type: uint8_t;
    class Node;
    class Sensor;
    class Relay;
    class Sink;
    class Nodes;

    // function predeclarations.
    coordinate_type distance(const Node&, const Node&);

    /* @class Node
     *
     * Class represents a wireless communication device,
     * which may be a sensor node, relay node or sink node.
     */
    class Node {
    public:
        /* data type of transmit power */
        typedef double       power_type;

        /* default transmit power */
        static const power_type    POWER;

        Node() = delete;
        virtual ~Node() = default;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;

        Coordinate coordinate(void) const { return _coordinate; }
        node_type  type(void) const { return _type; }
        power_type power(void) const { return _power; }
        hop_type   hop(void) const { return _hop; }
        id_type    id(void) const { return _id; }

        void set_coordinate(const Coordinate& coor) { _coordinate = coor; }
        void set_x(const coordinate_type& x) { _coordinate.set_x(x); }
        void set_y(const coordinate_type& y) { _coordinate.set_y(y); }
        void set_z(const coordinate_type& z) { _coordinate.set_z(z); }
        void set_power(const power_type& p) { _power = p; }

        bool operator==(const Node& n) const { return _id == n._id; }
        bool operator!=(const Node& n) const { return !(*this == n); }

    protected:
        Node(const Coordinate&, const node_type&, 
             const power_type&, const hop_type&, 
             const id_type&);

    protected:
        /* coordinate of this node */
        Coordinate       _coordinate;
        /* type of this node, i.e., sensor, relay or sink */
        node_type        _type;
        /* transmit power of this node */
        power_type       _power;
        /* hop constraint of this node */
        hop_type         _hop;
        /* the ID of this node, this is the universal identifier */
        id_type          _id;
    };

    const Node::power_type     Node::POWER  = 0.0;

    Node::Node(const Coordinate& coor, const node_type& n,
               const power_type& p, const hop_type& h,
               const id_type& i)
    : _coordinate(coor), _type(n), _power(p),
      _hop(h), _id(i) {}

    coordinate_type 
    distance(const Node& n1, const Node& n2) {
        return distance(n1.coordinate(), n2.coordinate());
    }

    /* @enum node_type
     *
     * enumerate type for identifying node type, i.e.,
     * SENSOR for sensor node, RELAY for relay node,
     * and SINK for sink node.
     */
    enum class node_type: uint8_t {
        SENSOR,
        RELAY,
        SINK
    };
    
    std::ostream&
    operator<<(std::ostream& os, const Node& n) {
        os << "[" << (n.type() == node_type::SENSOR ? "sensor" :
                     (n.type() == node_type::RELAY ? "relay" : "sink"))
                  << "ID->" << n.id() << ", power->" << n.power()
                  << ", " << n.coordinate() << ", hop->" << n.hop() << "]";

        return os;
    }

    /* @class Sensor
     *
     * Sensor node.
     */
    class Sensor: public Node {
    public:
        Sensor(const Coordinate& coor, const power_type& p,
               const hop_type& h, const id_type& i)
        : Node(coor, node_type::SENSOR, p, h, i) {}
        ~Sensor() = default;
    };

    /* @fn Relay
     *
     * Relay node.
     */
    class Relay: public Node {
    public:
        Relay(const Coordinate& coor, const power_type& p,
              const hop_type& h, const id_type& i)
        : Node(coor, node_type::RELAY, p, h, i) {}

        ~Relay() = default;
    };

    /* @fn Sink
     *
     * Sink node.
     */
    class Sink: public Node {
    public:
        Sink(const Coordinate& coor, const power_type& p,
             const hop_type& h, const id_type& i)
        : Node(coor, node_type::SINK, p, h, i) {}

        ~Sink() = default;
    };

    class Nodes {
    public:
        typedef std::vector<Node*>::value_type     value_type;
        typedef std::vector<Node*>::size_type      size_type;
        typedef std::vector<Node*>::iterator       iterator;
        typedef std::vector<Node*>::const_iterator const_iterator;

        Nodes(): nodes(std::vector<Node*>()) {}
        ~Nodes() { clear(); }

        iterator begin() noexcept { return nodes.begin(); }
        const_iterator begin() const noexcept { return nodes.begin(); }
        iterator end() noexcept { return nodes.end(); }
        const_iterator end() const noexcept { return nodes.end(); }

        size_type size() const { return nodes.size(); }
        bool empty() const { return nodes.empty(); }
        value_type& operator[](size_type n) { return nodes[n]; }
        const value_type& operator[](size_type n) const { return nodes[n]; }

        void push_back(const value_type& val) { nodes.push_back(val); }
        void push_back(value_type&& val) {nodes.push_back(std::move(val)); }
        void pop_back() { delete nodes[nodes.size() - 1]; nodes.pop_back(); }
        void clear() { while (!empty()) pop_back(); }

        Node* back() const { return nodes.back(); }
    private:
        std::vector<Node*> nodes;
    };
}

#endif
