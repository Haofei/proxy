// Boost.Geometry (aka GGL, Generic Geometry Library)
// QuickBook Example

// Copyright (c) 2011-2024 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[boost_range_strided
//` Shows how to use a Boost.Geometry ring, strided by Boost.Range adaptor

#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/adapted/boost_range/strided.hpp>

int main()
{
    using boost::adaptors::strided;

    using xy = boost::geometry::model::d2::point_xy<int>;
    boost::geometry::model::ring<xy> ring {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 0}};

    boost::geometry::correct(ring);

    std::cout
        << "Normal : " << boost::geometry::dsv(ring) << std::endl
        << "Strided: " << boost::geometry::dsv(ring | strided(2)) << std::endl;

    return 0;
}

//]

//[boost_range_strided_output
/*`
Output:
[pre
Normal : ((0, 0), (0, 1), (0, 2), (1, 2), (2, 2), (2, 0), (0, 0))
Strided: ((0, 0), (0, 2), (2, 2), (0, 0))
]
*/
//]
