// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2015-2024.
// Modifications copyright (c) 2015-2024, Oracle and/or its affiliates.
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_INTERSECTION_BOX_BOX_IMPLEMENTATION_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_INTERSECTION_BOX_BOX_IMPLEMENTATION_HPP


#include <boost/geometry/core/access.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace intersection
{

template <std::size_t Dimension, std::size_t DimensionCount>
struct intersection_box_box
{
    template
    <
        typename Box1, typename Box2,
        typename BoxOut,
        typename Strategy
    >
    static inline bool apply(Box1 const& box1,
            Box2 const& box2,
            BoxOut& box_out,
            Strategy const& strategy)
    {
        auto max1 = get<max_corner, Dimension>(box1);
        auto min2 = get<min_corner, Dimension>(box2);

        if (max1 < min2)
        {
            return false;
        }

        auto max2 = get<max_corner, Dimension>(box2);
        auto min1 = get<min_corner, Dimension>(box1);

        if (max2 < min1)
        {
            return false;
        }

        // Set dimensions of output coordinate
        set<min_corner, Dimension>(box_out, min1 < min2 ? min2 : min1);
        set<max_corner, Dimension>(box_out, max1 > max2 ? max2 : max1);

        return intersection_box_box<Dimension + 1, DimensionCount>
               ::apply(box1, box2, box_out, strategy);
    }
};

template <std::size_t DimensionCount>
struct intersection_box_box<DimensionCount, DimensionCount>
{
    template
    <
        typename Box1, typename Box2,
        typename BoxOut,
        typename Strategy
    >
    static inline bool apply(Box1 const&, Box2 const&, BoxOut&, Strategy const&)
    {
        return true;
    }
};


}} // namespace detail::intersection
#endif // DOXYGEN_NO_DETAIL

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_INTERSECTION_BOX_BOX_IMPLEMENTATION_HPP
