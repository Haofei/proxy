// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2015 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2015 Mateusz Loskot, London, UK.
// Copyright (c) 2023-2024 Adam Wulkiewicz, Lodz, Poland.

// This file was modified by Oracle on 2018-2023.
// Modifications copyright (c) 2018-2023 Oracle and/or its affiliates.
// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP
#define BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP

#include <cstddef>
#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
#include <iostream>
#endif
#include <set>
#include <vector>

#include <boost/core/addressof.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/value_type.hpp>

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/clear.hpp>
#include <boost/geometry/algorithms/convert.hpp>
#include <boost/geometry/algorithms/detail/dummy_geometries.hpp>
#include <boost/geometry/algorithms/detail/equals/point_point.hpp>
#include <boost/geometry/algorithms/detail/visit.hpp>
#include <boost/geometry/algorithms/not_implemented.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <boost/geometry/algorithms/perimeter.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/closure.hpp>
#include <boost/geometry/core/exterior_ring.hpp>
#include <boost/geometry/core/interior_rings.hpp>
#include <boost/geometry/core/mutable_range.hpp>
#include <boost/geometry/core/tags.hpp>
#include <boost/geometry/core/visit.hpp>

#include <boost/geometry/geometries/adapted/boost_variant.hpp> // For backward compatibility
#include <boost/geometry/geometries/concepts/check.hpp>

#include <boost/geometry/strategies/default_strategy.hpp>
#include <boost/geometry/strategies/detail.hpp>
#include <boost/geometry/strategies/distance/comparable.hpp>
#include <boost/geometry/strategies/simplify/cartesian.hpp>
#include <boost/geometry/strategies/simplify/geographic.hpp>
#include <boost/geometry/strategies/simplify/spherical.hpp>

#include <boost/geometry/util/constexpr.hpp>
#include <boost/geometry/util/type_traits_std.hpp>

#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
#include <boost/geometry/io/dsv/write.hpp>
#endif

namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace simplify
{

/*!
\brief Small wrapper around a point, with an extra member "included"
\details
    It has a const-reference to the original point (so no copy here)
\tparam the enclosed point type
*/
template <typename Point>
struct douglas_peucker_point
{
    typedef Point point_type;

    Point const* p;
    bool included;

    inline douglas_peucker_point(Point const& ap)
        : p(boost::addressof(ap))
        , included(false)
    {}
};

/*!
\brief Implements the simplify algorithm.
\details The douglas_peucker policy simplifies a linestring, ring or
    vector of points using the well-known Douglas-Peucker algorithm.
\note This strategy uses itself a point-segment potentially comparable
    distance strategy
\author Barend and Maarten, 1995/1996
\author Barend, revised for Generic Geometry Library, 2008
*/

/*
For the algorithm, see for example:
 - http://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
 - http://www2.dcs.hull.ac.uk/CISRG/projects/Royal-Inst/demos/dp.html
*/
class douglas_peucker
{
    template <typename Iterator, typename Distance, typename PSDistanceStrategy>
    static inline void consider(Iterator begin,
                                Iterator end,
                                Distance const& max_dist,
                                int& n,
                                PSDistanceStrategy const& ps_distance_strategy)
    {
        typedef typename std::iterator_traits<Iterator>::value_type::point_type point_type;
        typedef decltype(ps_distance_strategy.apply(std::declval<point_type>(),
                            std::declval<point_type>(), std::declval<point_type>())) distance_type;

        std::size_t size = end - begin;

        // size must be at least 3
        // because we want to consider a candidate point in between
        if (size <= 2)
        {
#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
            if (begin != end)
            {
                std::cout << "ignore between " << dsv(*(begin->p))
                    << " and " << dsv(*((end - 1)->p))
                    << " size=" << size << std::endl;
            }
            std::cout << "return because size=" << size << std::endl;
#endif
            return;
        }

        Iterator last = end - 1;

#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
        std::cout << "find between " << dsv(*(begin->p))
            << " and " << dsv(*(last->p))
            << " size=" << size << std::endl;
#endif


        // Find most far point, compare to the current segment
        //geometry::segment<Point const> s(begin->p, last->p);
        distance_type md(-1.0); // any value < 0
        Iterator candidate = end;
        for (Iterator it = begin + 1; it != last; ++it)
        {
            distance_type dist = ps_distance_strategy.apply(*(it->p), *(begin->p), *(last->p));

#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
            std::cout << "consider " << dsv(*(it->p))
                << " at " << double(dist)
                << ((dist > max_dist) ? " maybe" : " no")
                << std::endl;

#endif
            if (md < dist)
            {
                md = dist;
                candidate = it;
            }
        }

        // If a point is found, set the include flag
        // and handle segments in between recursively
        if (max_dist < md && candidate != end)
        {
#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
            std::cout << "use " << dsv(candidate->p) << std::endl;
#endif

            candidate->included = true;
            n++;

            consider(begin, candidate + 1, max_dist, n, ps_distance_strategy);
            consider(candidate, end, max_dist, n, ps_distance_strategy);
        }
    }

    template
    <
        typename Range, typename OutputIterator, typename Distance,
        typename PSDistanceStrategy
    >
    static inline OutputIterator apply_(Range const& range,
                                        OutputIterator out,
                                        Distance const& max_distance,
                                        PSDistanceStrategy const& ps_distance_strategy)
    {
#ifdef BOOST_GEOMETRY_DEBUG_DOUGLAS_PEUCKER
            std::cout << "max distance: " << max_distance
                      << std::endl << std::endl;
#endif

        typedef typename boost::range_value<Range>::type point_type;
        typedef douglas_peucker_point<point_type> dp_point_type;

        // Copy coordinates, a vector of references to all points
        std::vector<dp_point_type> ref_candidates(boost::begin(range),
                                                  boost::end(range));

        // Include first and last point of line,
        // they are always part of the line
        int n = 2;
        ref_candidates.front().included = true;
        ref_candidates.back().included = true;

        // Get points, recursively, including them if they are further away
        // than the specified distance
        consider(boost::begin(ref_candidates), boost::end(ref_candidates), max_distance, n,
                 ps_distance_strategy);

        // Copy included elements to the output
        for (auto it = boost::begin(ref_candidates); it != boost::end(ref_candidates); ++it)
        {
            if (it->included)
            {
                // copy-coordinates does not work because OutputIterator
                // does not model Point (??)
                //geometry::convert(*(it->p), *out);
                *out = *(it->p);
                ++out;
            }
        }
        return out;
    }

public:
    template <typename Range, typename OutputIterator, typename Distance, typename Strategies>
    static inline OutputIterator apply(Range const& range,
                                       OutputIterator out,
                                       Distance const& max_distance,
                                       Strategies const& strategies)
    {
        typedef typename boost::range_value<Range>::type point_type;
        typedef decltype(strategies.distance(detail::dummy_point(), detail::dummy_segment())) distance_strategy_type;

        typedef typename strategy::distance::services::comparable_type
            <
                distance_strategy_type
            >::type comparable_distance_strategy_type;

        comparable_distance_strategy_type cstrategy = strategy::distance::services::get_comparable
            <
                distance_strategy_type
            >::apply(strategies.distance(detail::dummy_point(), detail::dummy_segment()));

        return apply_(range, out,
                      strategy::distance::services::result_from_distance
                          <
                              comparable_distance_strategy_type, point_type, point_type
                          >::apply(cstrategy, max_distance),
                      cstrategy);
    }
};


template <typename Range, typename Strategies>
inline bool is_degenerate(Range const& range, Strategies const& strategies)
{
    return boost::size(range) == 2
        && detail::equals::equals_point_point(geometry::range::front(range),
                                              geometry::range::back(range),
                                              strategies);
}

struct simplify_range_insert
{
    template
    <
        typename Range, typename OutputIterator, typename Distance,
        typename Impl, typename Strategies
    >
    static inline void apply(Range const& range, OutputIterator out,
                             Distance const& max_distance,
                             Impl const& impl,
                             Strategies const& strategies)
    {
        if (is_degenerate(range, strategies))
        {
            std::copy(boost::begin(range), boost::begin(range) + 1, out);
        }
        else if (boost::size(range) <= 2 || max_distance < 0)
        {
            std::copy(boost::begin(range), boost::end(range), out);
        }
        else
        {
            impl.apply(range, out, max_distance, strategies);
        }
    }
};


struct simplify_copy_assign
{
    template
    <
        typename In, typename Out, typename Distance,
        typename Impl, typename Strategies
    >
    static inline void apply(In const& in, Out& out,
                             Distance const& ,
                             Impl const& ,
                             Strategies const& )
    {
        out = in;
    }
};


struct simplify_copy
{
    template
    <
        typename RangeIn, typename RangeOut, typename Distance,
        typename Impl, typename Strategies
    >
    static inline void apply(RangeIn const& range, RangeOut& out,
                             Distance const& ,
                             Impl const& ,
                             Strategies const& )
    {
        std::copy(boost::begin(range), boost::end(range),
                  geometry::range::back_inserter(out));
    }
};


template <std::size_t MinimumToUseStrategy>
struct simplify_range
{
    template
    <
        typename RangeIn, typename RangeOut, typename Distance,
        typename Impl, typename Strategies
    >
    static inline void apply(RangeIn const& range, RangeOut& out,
                             Distance const& max_distance,
                             Impl const& impl,
                             Strategies const& strategies)
    {
        // For a RING:
        // Note that, especially if max_distance is too large,
        // the output ring might be self intersecting while the input ring is
        // not, although chances are low in normal polygons

        if (boost::size(range) <= MinimumToUseStrategy || max_distance < 0)
        {
            simplify_copy::apply(range, out, max_distance, impl, strategies);
        }
        else
        {
            simplify_range_insert::apply(range, geometry::range::back_inserter(out),
                                         max_distance, impl, strategies);
        }

        // Verify the two remaining points are equal. If so, remove one of them.
        // This can cause the output being under the minimum size
        if (is_degenerate(out, strategies))
        {
            range::resize(out, 1);
        }
    }
};

struct simplify_ring
{
private :
    template <typename Area>
    static inline int area_sign(Area const& area)
    {
        return area > 0 ? 1 : area < 0 ? -1 : 0;
    }

    template <typename Ring, typename Strategies>
    static std::size_t get_opposite(std::size_t index, Ring const& ring,
                                    Strategies const& strategies)
    {
        // TODO: Instead of calling the strategy call geometry::comparable_distance() ?

        auto const cdistance_strategy = strategies::distance::detail::make_comparable(strategies)
            .distance(detail::dummy_point(), detail::dummy_point());

        using point_type = geometry::point_type_t<Ring>;
        using cdistance_type = decltype(cdistance_strategy.apply(
            std::declval<point_type>(), std::declval<point_type>()));

        // Verify if it is NOT the case that all points are less than the
        // simplifying distance. If so, output is empty.
        cdistance_type max_cdistance(-1);

        point_type const& point = range::at(ring, index);
        std::size_t i = 0;
        for (auto it = boost::begin(ring); it != boost::end(ring); ++it, ++i)
        {
            cdistance_type const cdistance = cdistance_strategy.apply(*it, point);
            if (cdistance > max_cdistance)
            {
                max_cdistance = cdistance;
                index = i;
            }
        }
        return index;
    }

public :
    template
    <
        typename RingIn, typename RingOut,
        typename Distance, typename Impl, typename Strategies
    >
    static inline void apply(RingIn const& ring, RingOut& out, Distance const& max_distance,
                             Impl const& impl, Strategies const& strategies)
    {
        std::size_t const size = boost::size(ring);
        if (size == 0)
        {
            return;
        }

        constexpr bool is_closed_in = geometry::closure<RingIn>::value == closed;
        constexpr bool is_closed_out = geometry::closure<RingOut>::value == closed;
        constexpr bool is_clockwise_in = geometry::point_order<RingIn>::value == clockwise;
        constexpr bool is_clockwise_out = geometry::point_order<RingOut>::value == clockwise;

        // TODO: instead of area() use calculate_point_order() ?

        int const input_sign = area_sign(geometry::area(ring, strategies));

        std::set<std::size_t> visited_indexes;

        // Rotate it into a copied vector
        // (vector, because source type might not support rotation)
        // (duplicate end point will be simplified away)
        using point_type = geometry::point_type_t<RingIn>;

        std::vector<point_type> rotated;
        rotated.reserve(size + 1); // 1 because open rings are closed

        // Closing point (but it will not start here)
        std::size_t index = 0;

        // Iterate (usually one iteration is enough)
        for (std::size_t iteration = 0; iteration < 4u; iteration++)
        {
            // Always take the opposite. Opposite guarantees that no point
            // "halfway" is chosen, creating an artefact (very narrow triangle)
            // Iteration 0: opposite to closing point (1/2, = on convex hull)
            //              (this will start simplification with that point
            //               and its opposite ~0)
            // Iteration 1: move a quarter on that ring, then opposite to 1/4
            //              (with its opposite 3/4)
            // Iteration 2: move an eight on that ring, then opposite (1/8)
            // Iteration 3: again move a quarter, then opposite (7/8)
            // So finally 8 "sides" of the ring have been examined (if it were
            // a semi-circle). Most probably, there are only 0 or 1 iterations.
            switch (iteration)
            {
                case 1 : index = (index + size / 4) % size; break;
                case 2 : index = (index + size / 8) % size; break;
                case 3 : index = (index + size / 4) % size; break;
            }
            index = get_opposite(index, ring, strategies);

            if (visited_indexes.count(index) > 0)
            {
                // Avoid trying the same starting point more than once
                continue;
            }

            // Do not duplicate the closing point
            auto rot_end = boost::end(ring);
            std::size_t rot_index = index;
            if BOOST_GEOMETRY_CONSTEXPR (is_closed_in)
            {
                if (size > 1)
                {
                    --rot_end;
                    if (rot_index == size - 1) { rot_index = 0; }
                }
            }

            std::rotate_copy(boost::begin(ring), range::pos(ring, rot_index),
                             rot_end, std::back_inserter(rotated));

            // Close the rotated copy
            rotated.push_back(range::at(ring, rot_index));

            simplify_range<0>::apply(rotated, out, max_distance, impl, strategies);

            // Open output if needed
            if BOOST_GEOMETRY_CONSTEXPR (! is_closed_out)
            {
                if (boost::size(out) > 1)
                {
                    range::pop_back(out);
                }
            }

            // TODO: instead of area() use calculate_point_order() ?

            // Verify that what was positive, stays positive (or goes to 0)
            // and what was negative stays negative (or goes to 0)
            int const output_sign = area_sign(geometry::area(out, strategies));
            if (output_sign == input_sign)
            {
                // Result is considered as satisfactory (usually this is the
                // first iteration - only for small rings, having a scale
                // similar to simplify_distance, next iterations are tried
                return;
            }

            // Original is simplified away. Possibly there is a solution
            // when another starting point is used
            geometry::clear(out);

            if (iteration == 0
                && geometry::perimeter(ring, strategies) < 3 * max_distance)
            {
                // Check if it is useful to iterate. A minimal triangle has a
                // perimeter of a bit more than 3 times the simplify distance
                return;
            }

            // Prepare next try
            visited_indexes.insert(index);
            rotated.clear();
        }

        if BOOST_GEOMETRY_CONSTEXPR (is_clockwise_in != is_clockwise_out)
        {
            std::reverse(boost::begin(out), boost::end(out));
        }
    }
};


struct simplify_polygon
{
private:

    template
    <
        typename IteratorIn,
        typename InteriorRingsOut,
        typename Distance,
        typename Impl,
        typename Strategies
    >
    static inline void iterate(IteratorIn begin, IteratorIn end,
                               InteriorRingsOut& interior_rings_out,
                               Distance const& max_distance,
                               Impl const& impl, Strategies const& strategies)
    {
        typedef typename boost::range_value<InteriorRingsOut>::type single_type;
        for (IteratorIn it = begin; it != end; ++it)
        {
            single_type out;
            simplify_ring::apply(*it, out, max_distance, impl, strategies);
            if (! geometry::is_empty(out))
            {
                range::push_back(interior_rings_out, std::move(out));
            }
        }
    }

    template
    <
        typename InteriorRingsIn,
        typename InteriorRingsOut,
        typename Distance,
        typename Impl,
        typename Strategies
    >
    static inline void apply_interior_rings(InteriorRingsIn const& interior_rings_in,
                                            InteriorRingsOut& interior_rings_out,
                                            Distance const& max_distance,
                                            Impl const& impl, Strategies const& strategies)
    {
        range::clear(interior_rings_out);

        iterate(boost::begin(interior_rings_in), boost::end(interior_rings_in),
                interior_rings_out,
                max_distance,
                impl, strategies);
    }

public:
    template
    <
        typename PolygonIn, typename PolygonOut,
        typename Distance, typename Impl, typename Strategies
    >
    static inline void apply(PolygonIn const& poly_in, PolygonOut& poly_out,
                             Distance const& max_distance,
                             Impl const& impl, Strategies const& strategies)
    {
        // Note that if there are inner rings, and distance is too large,
        // they might intersect with the outer ring in the output,
        // while it didn't in the input.
        simplify_ring::apply(exterior_ring(poly_in), exterior_ring(poly_out),
                             max_distance, impl, strategies);

        apply_interior_rings(interior_rings(poly_in), interior_rings(poly_out),
                             max_distance, impl, strategies);
    }
};


template<typename Policy>
struct simplify_multi
{
    template
    <
        typename MultiGeometryIn, typename MultiGeometryOut,
        typename Distance, typename Impl, typename Strategies
    >
    static inline void apply(MultiGeometryIn const& multi, MultiGeometryOut& out,
                             Distance const& max_distance,
                             Impl const& impl, Strategies const& strategies)
    {
        range::clear(out);

        using single_type = typename boost::range_value<MultiGeometryOut>::type;

        for (auto it = boost::begin(multi); it != boost::end(multi); ++it)
        {
            single_type single_out;
            Policy::apply(*it, single_out, max_distance, impl, strategies);
            if (! geometry::is_empty(single_out))
            {
                range::push_back(out, std::move(single_out));
            }
        }
    }
};


template <typename Geometry>
struct has_same_tag_as
{
    template <typename OtherGeometry>
    struct pred
        : std::is_same<geometry::tag_t<Geometry>, geometry::tag_t<OtherGeometry>>
    {};
};

template <typename StaticGeometryIn, typename DynamicGeometryOut>
struct static_geometry_type
{
    using type = typename util::sequence_find_if
        <
            typename traits::geometry_types<DynamicGeometryOut>::type,
            detail::simplify::has_same_tag_as<StaticGeometryIn>::template pred
        >::type;

    BOOST_GEOMETRY_STATIC_ASSERT(
        (! std::is_void<type>::value),
        "Unable to find corresponding geometry in GeometryOut",
        StaticGeometryIn, DynamicGeometryOut);
};


}} // namespace detail::simplify
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename GeometryIn,
    typename GeometryOut,
    typename TagIn = tag_t<GeometryIn>,
    typename TagOut = tag_t<GeometryOut>
>
struct simplify: not_implemented<TagIn, TagOut>
{};

template <typename PointIn, typename PointOut>
struct simplify<PointIn, PointOut, point_tag, point_tag>
{
    template <typename Distance, typename Impl, typename Strategy>
    static inline void apply(PointIn const& point, PointOut& out, Distance const& ,
                             Impl const& , Strategy const& )
    {
        geometry::convert(point, out);
    }
};

template <typename SegmentIn, typename SegmentOut>
struct simplify<SegmentIn, SegmentOut, segment_tag, segment_tag>
    : detail::simplify::simplify_copy_assign
{};

template <typename BoxIn, typename BoxOut>
struct simplify<BoxIn, BoxOut, box_tag, box_tag>
    : detail::simplify::simplify_copy_assign
{};

// Linestring, keep 2 points (unless those points are the same)
template <typename LinestringIn, typename LinestringOut>
struct simplify<LinestringIn, LinestringOut, linestring_tag, linestring_tag>
    : detail::simplify::simplify_range<2>
{};

template <typename RingIn, typename RingOut>
struct simplify<RingIn, RingOut, ring_tag, ring_tag>
    : detail::simplify::simplify_ring
{};

template <typename PolygonIn, typename PolygonOut>
struct simplify<PolygonIn, PolygonOut, polygon_tag, polygon_tag>
    : detail::simplify::simplify_polygon
{};

template <typename MultiPointIn, typename MultiPointOut>
struct simplify<MultiPointIn, MultiPointOut, multi_point_tag, multi_point_tag>
    : detail::simplify::simplify_copy
{};

template <typename MultiLinestringIn, typename MultiLinestringOut>
struct simplify<MultiLinestringIn, MultiLinestringOut, multi_linestring_tag, multi_linestring_tag>
    : detail::simplify::simplify_multi<detail::simplify::simplify_range<2> >
{};

template <typename MultiPolygonIn, typename MultiPolygonOut>
struct simplify<MultiPolygonIn, MultiPolygonOut, multi_polygon_tag, multi_polygon_tag>
    : detail::simplify::simplify_multi<detail::simplify::simplify_polygon>
{};


template
<
    typename Geometry,
    typename Tag = tag_t<Geometry>
>
struct simplify_insert: not_implemented<Tag>
{};


template <typename Linestring>
struct simplify_insert<Linestring, linestring_tag>
    : detail::simplify::simplify_range_insert
{};

template <typename Ring>
struct simplify_insert<Ring, ring_tag>
    : detail::simplify::simplify_range_insert
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


namespace resolve_strategy
{

template
<
    typename Strategies,
    bool IsUmbrella = strategies::detail::is_umbrella_strategy<Strategies>::value
>
struct simplify
{
    template <typename GeometryIn, typename GeometryOut, typename Distance>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             Strategies const& strategies)
    {
        dispatch::simplify
            <
                GeometryIn, GeometryOut
            >::apply(geometry, out, max_distance,
                     detail::simplify::douglas_peucker(),
                     strategies);
    }
};

template <typename Strategy>
struct simplify<Strategy, false>
{
    template <typename GeometryIn, typename GeometryOut, typename Distance>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             Strategy const& strategy)
    {
        using strategies::simplify::services::strategy_converter;

        simplify
            <
                decltype(strategy_converter<Strategy>::get(strategy))
            >::apply(geometry, out, max_distance,
                     strategy_converter<Strategy>::get(strategy));
    }
};

template <>
struct simplify<default_strategy, false>
{
    template <typename GeometryIn, typename GeometryOut, typename Distance>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             default_strategy)
    {
        // NOTE: Alternatively take two geometry types in default_strategy
        using cs_tag1_t = geometry::cs_tag_t<GeometryIn>;
        using cs_tag2_t = geometry::cs_tag_t<GeometryOut>;
        BOOST_GEOMETRY_STATIC_ASSERT(
            (std::is_same<cs_tag1_t, cs_tag2_t>::value),
            "Incompatible coordinate systems",
            cs_tag1_t, cs_tag2_t);

        typedef typename strategies::simplify::services::default_strategy
            <
                GeometryIn
            >::type strategy_type;

        simplify
            <
                strategy_type
            >::apply(geometry, out, max_distance, strategy_type());
    }
};

template
<
    typename Strategies,
    bool IsUmbrella = strategies::detail::is_umbrella_strategy<Strategies>::value
>
struct simplify_insert
{
    template<typename Geometry, typename OutputIterator, typename Distance>
    static inline void apply(Geometry const& geometry,
                             OutputIterator& out,
                             Distance const& max_distance,
                             Strategies const& strategies)
    {
        dispatch::simplify_insert
            <
                Geometry
            >::apply(geometry, out, max_distance,
                     detail::simplify::douglas_peucker(),
                     strategies);
    }
};

template <typename Strategy>
struct simplify_insert<Strategy, false>
{
    template<typename Geometry, typename OutputIterator, typename Distance>
    static inline void apply(Geometry const& geometry,
                             OutputIterator& out,
                             Distance const& max_distance,
                             Strategy const& strategy)
    {
        using strategies::simplify::services::strategy_converter;

        simplify_insert
            <
                decltype(strategy_converter<Strategy>::get(strategy))
            >::apply(geometry, out, max_distance,
                     strategy_converter<Strategy>::get(strategy));
    }
};

template <>
struct simplify_insert<default_strategy, false>
{
    template <typename Geometry, typename OutputIterator, typename Distance>
    static inline void apply(Geometry const& geometry,
                             OutputIterator& out,
                             Distance const& max_distance,
                             default_strategy)
    {
        typedef typename strategies::simplify::services::default_strategy
            <
                Geometry
            >::type strategy_type;

        simplify_insert
            <
                strategy_type
            >::apply(geometry, out, max_distance, strategy_type());
    }
};

} // namespace resolve_strategy


namespace resolve_dynamic {

template
<
    typename GeometryIn, typename GeometryOut,
    typename TagIn = tag_t<GeometryIn>,
    typename TagOut = tag_t<GeometryOut>
>
struct simplify
{
    template <typename Distance, typename Strategy>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             Strategy const& strategy)
    {
        resolve_strategy::simplify<Strategy>::apply(geometry, out, max_distance, strategy);
    }
};

template <typename GeometryIn, typename GeometryOut>
struct simplify<GeometryIn, GeometryOut, dynamic_geometry_tag, dynamic_geometry_tag>
{
    template <typename Distance, typename Strategy>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             Strategy const& strategy)
    {
        traits::visit<GeometryIn>::apply([&](auto const& g)
        {
            using geom_t = util::remove_cref_t<decltype(g)>;
            using detail::simplify::static_geometry_type;
            using geom_out_t = typename static_geometry_type<geom_t, GeometryOut>::type;
            geom_out_t o;
            simplify<geom_t, geom_out_t>::apply(g, o, max_distance, strategy);
            out = std::move(o);
        }, geometry);
    }
};

template <typename GeometryIn, typename GeometryOut>
struct simplify<GeometryIn, GeometryOut, geometry_collection_tag, geometry_collection_tag>
{
    template <typename Distance, typename Strategy>
    static inline void apply(GeometryIn const& geometry,
                             GeometryOut& out,
                             Distance const& max_distance,
                             Strategy const& strategy)
    {
        detail::visit_breadth_first([&](auto const& g)
        {
            using geom_t = util::remove_cref_t<decltype(g)>;
            using detail::simplify::static_geometry_type;
            using geom_out_t = typename static_geometry_type<geom_t, GeometryOut>::type;
            geom_out_t o;
            simplify<geom_t, geom_out_t>::apply(g, o, max_distance, strategy);
            traits::emplace_back<GeometryOut>::apply(out, std::move(o));
            return true;
        }, geometry);
    }
};

} // namespace resolve_dynamic


/*!
\brief Simplify a geometry using a specified strategy
\ingroup simplify
\tparam Geometry \tparam_geometry
\tparam GeometryOut The output geometry
\tparam Distance A numerical distance measure
\tparam Strategy A type fulfilling a SimplifyStrategy concept
\param geometry input geometry, to be simplified
\param out output geometry, simplified version of the input geometry
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed
\param strategy simplify strategy to be used for simplification
\note The simplification is done with Douglas-Peucker algorithm

\image html svg_simplify_country.png "The image below presents the simplified country"
\qbk{distinguish,with strategy}
*/
template<typename Geometry, typename GeometryOut, typename Distance, typename Strategy>
inline void simplify(Geometry const& geometry, GeometryOut& out,
                     Distance const& max_distance, Strategy const& strategy)
{
    concepts::check<Geometry const>();
    concepts::check<GeometryOut>();

    geometry::clear(out);

    resolve_dynamic::simplify<Geometry, GeometryOut>::apply(geometry, out, max_distance, strategy);
}




/*!
\brief Simplify a geometry
\ingroup simplify
\tparam Geometry \tparam_geometry
\tparam GeometryOut The output geometry
\tparam Distance \tparam_numeric
\param geometry input geometry, to be simplified
\param out output geometry, simplified version of the input geometry
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed
\note The simplification is done with Douglas-Peucker algorithm

\qbk{[include reference/algorithms/simplify.qbk]}
 */
template<typename Geometry, typename GeometryOut, typename Distance>
inline void simplify(Geometry const& geometry, GeometryOut& out,
                     Distance const& max_distance)
{
    concepts::check<Geometry const>();
    concepts::check<GeometryOut>();

    geometry::simplify(geometry, out, max_distance, default_strategy());
}


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace simplify
{


/*!
\brief Simplify a geometry, using an output iterator
    and a specified strategy
\ingroup simplify
\tparam Geometry \tparam_geometry
\param geometry input geometry, to be simplified
\param out output iterator, outputs all simplified points
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed
\param strategy simplify strategy to be used for simplification

\qbk{distinguish,with strategy}
\qbk{[include reference/algorithms/simplify.qbk]}
*/
template<typename Geometry, typename OutputIterator, typename Distance, typename Strategy>
inline void simplify_insert(Geometry const& geometry, OutputIterator out,
                            Distance const& max_distance, Strategy const& strategy)
{
    concepts::check<Geometry const>();

    resolve_strategy::simplify_insert<Strategy>::apply(geometry, out, max_distance, strategy);
}

/*!
\brief Simplify a geometry, using an output iterator
\ingroup simplify
\tparam Geometry \tparam_geometry
\param geometry input geometry, to be simplified
\param out output iterator, outputs all simplified points
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed

\qbk{[include reference/algorithms/simplify_insert.qbk]}
 */
template<typename Geometry, typename OutputIterator, typename Distance>
inline void simplify_insert(Geometry const& geometry, OutputIterator out,
                            Distance const& max_distance)
{
    // Concept: output point type = point type of input geometry
    concepts::check<Geometry const>();
    concepts::check<point_type_t<Geometry>>();

    simplify_insert(geometry, out, max_distance, default_strategy());
}

}} // namespace detail::simplify
#endif // DOXYGEN_NO_DETAIL



}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP
