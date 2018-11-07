#ifndef _QOSRNP_COORDINATE_H
#define _QOSRNP_COORDINATE_H

#include <iostream>
#include <utility>     // move()
#include <cmath>       // sqrt()

#include "header.h"

namespace qosrnp {
    class Coordinate {
    public:
        Coordinate(const coordinate_type&, const coordinate_type&, 
                   const coordinate_type&);
        Coordinate(const Coordinate&);
        Coordinate(Coordinate&&);

        Coordinate& operator=(const Coordinate&);
        Coordinate& operator=(Coordinate&&);
        Coordinate  operator+(const Coordinate&) const;
        Coordinate  operator-(const Coordinate&) const;
        Coordinate& operator+=(const Coordinate&);
        Coordinate& operator-=(const Coordinate&);
        bool        operator==(const Coordinate&) const;
        bool        operator!=(const Coordinate&) const;

        coordinate_type x(void) const { return _x; }
        coordinate_type y(void) const { return _y; }
        coordinate_type z(void) const { return _z; }

        void set_x(const coordinate_type& a) { _x = a; }
        void set_y(const coordinate_type& b) { _y = b; }
        void set_z(const coordinate_type& c) { _z = c; }

    private:
        coordinate_type    _x;
        coordinate_type    _y;
        coordinate_type    _z;
    };

    Coordinate::Coordinate(const coordinate_type& a = coordinate_type(),
               const coordinate_type& b = coordinate_type(),
               const coordinate_type& c = coordinate_type())
    : _x(a), _y(b), _z(c) {}

    Coordinate::Coordinate(const Coordinate& coor)
    : _x(coor._x), _y(coor._y), _z(coor._z) {}

    Coordinate::Coordinate(Coordinate&& coor)
    : _x(std::move(coor._x)), _y(std::move(coor._y)), 
      _z(std::move(coor._z)) {}

    Coordinate&
    Coordinate::operator=(const Coordinate& coor) {
        _x = coor._x; _y = coor._y; _z = coor._z;
        return *this;
    }

    Coordinate&
    Coordinate::operator=(Coordinate&& coor) {
        _x = std::move(coor._x);
        _y = std::move(coor._y);
        _z = std::move(coor._z);
        return *this;
    }

    Coordinate
    Coordinate::operator+(const Coordinate& rhs) const {
        return Coordinate(_x + rhs._x, _y + rhs._y, _z + rhs._z);
    }

    Coordinate
    Coordinate::operator-(const Coordinate& rhs) const {
        return Coordinate(_x - rhs._x, _y - rhs._y, _z - rhs._z);
    }

    Coordinate&
    Coordinate::operator+=(const Coordinate& coor) {
        _x += coor._x; _y += coor._y; _z += coor._z;
        return *this;
    }

    Coordinate&
    Coordinate::operator-=(const Coordinate& coor) {
        _x -= coor._x; _y -= coor._y; _z -= coor._z;
        return *this;
    }

    bool
    Coordinate::operator==(const Coordinate& rhs) const {
        return _x == rhs._x && _y == rhs._y && _z == rhs._z;
    }

    bool
    Coordinate::operator!=(const Coordinate& rhs) const {
        return !(*this == rhs);
    }

    coordinate_type 
    distance(const Coordinate& n1, const Coordinate& n2) {
        coordinate_type tmp_x, tmp_y, tmp_z;

        tmp_x = std::fabs(n1.x() - n2.x());
        tmp_y = std::fabs(n1.y() - n2.y());
        tmp_z = std::fabs(n1.z() - n2.z());

        return std::sqrt(std::pow(tmp_x, 2) +
                         std::pow(tmp_y, 2) +
                         std::pow(tmp_z, 2));
    }

    std::ostream& 
    operator<<(std::ostream& os, const Coordinate& coor) {
        os << "(" << coor.x() << ", "
           << coor.y() << ", " << coor.z() << ")";
        return os;
    }
}

#endif
