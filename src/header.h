#ifndef _QOSRNP_HEADER_H
#define _QOSRNP_HEADER_H

#include <cstdint>      // uintx_t

namespace qosrnp {
    /* data type of coordinate */
    typedef double        coordinate_type;
    /* data type of ID */
    typedef uint16_t      id_type;
    /* data type of hop count */
    typedef uint16_t       hop_type;
    /* data type of node degree */
    typedef uint16_t       degree_type;
    /* data type of wireless link quality */
    typedef double        quality_type;

    /* constraint on node degree */
    const degree_type     DEGREE_CONSTRAINT    = 10;
    /* constraint on wireless link quality */
    const quality_type    QUALITY_CONSTRAINT   = 0.95;
}

#endif
