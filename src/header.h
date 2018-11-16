#ifndef _QOSRNP_HEADER_H
#define _QOSRNP_HEADER_H

#include <cstdlib>
#include <cstdint>      // uintx_t

namespace qosrnp {
    #define   GENERATION       200
    #define   POPULATION       10
    #define   CDL_NUM          400

    /* data type of coordinate */
    typedef double        coordinate_type;
    /* data type of ID */
    typedef int32_t       id_type;
    /* data type of hop count */
    typedef int32_t       hop_type;
    /* data type of node degree */
    typedef int32_t       degree_type;
    /* data type of wireless link quality */
    typedef double        quality_type;
    /* data type of size */
    typedef size_t        size_type;

    /* constraint on node degree */
    const degree_type     DEGREE_CONSTRAINT    = 10;
    /* constraint on wireless link quality */
    const quality_type    QUALITY_CONSTRAINT   = 0.95;

    unsigned int     sensor_num = 100;
    unsigned int     sink_num = 1;
    unsigned int     relay_num = 400;

    double           power = 20.0;
    unsigned int     hop_constraint = 15;
}

#endif
