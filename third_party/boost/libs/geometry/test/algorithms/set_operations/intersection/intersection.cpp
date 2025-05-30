// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2015 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2015 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2015-2022.
// Modifications copyright (c) 2015-2022, Oracle and/or its affiliates.
// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <climits>
#include <iostream>
#include <string>

#include <boost/config.hpp>
#include <boost/core/ignore_unused.hpp>

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/algorithms/detail/has_self_intersections.hpp>

#include <boost/geometry/util/condition.hpp>
#include <boost/geometry/util/constexpr.hpp>
#include <boost/geometry/util/rational.hpp>

#include "test_intersection.hpp"
#include <algorithms/test_overlay.hpp>

#include <algorithms/overlay/overlay_cases.hpp>

#include <test_common/test_point.hpp>
#include <test_common/with_pointer.hpp>
#include <test_geometries/custom_segment.hpp>


BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(std::vector)

#define TEST_INTERSECTION(caseid, clips, points, area) \
    (test_one<Polygon, Polygon, Polygon>) \
    ( #caseid, caseid[0], caseid[1], clips, points, area)

#define TEST_INTERSECTION_REV(caseid, clips, points, area) \
    (test_one<Polygon, Polygon, Polygon>) \
    ( #caseid "_rev", caseid[1], caseid[0], clips, points, area)

#define TEST_INTERSECTION_WITH(caseid, index1, index2, \
     clips, points, area, settings) \
    (test_one<Polygon, Polygon, Polygon>) \
    ( #caseid "_" #index1 "_" #index2, caseid[index1], caseid[index2], \
     clips, points, area, settings)

template <typename Polygon>
void test_areal()
{
    test_one<Polygon, Polygon, Polygon>("simplex_with_empty_1",
        simplex_normal[0], polygon_empty,
        0, 0, 0.0);
    test_one<Polygon, Polygon, Polygon>("simplex_with_empty_2",
        polygon_empty, simplex_normal[0],
        0, 0, 0.0);

    test_one<Polygon, Polygon, Polygon>("simplex_normal",
        simplex_normal[0], simplex_normal[1],
        1, 7, 5.47363293);
    test_one<Polygon, Polygon, Polygon>("star_ring", example_star, example_ring,
        1, 18, 2.80983);

    test_one<Polygon, Polygon, Polygon>("star_poly", example_star, example_polygon,
        1, 0, // CLN: 23 points, other types: 22 point (one is merged)
        2.5020508);
    test_one<Polygon, Polygon, Polygon>("first_within_second1",
        first_within_second[0], first_within_second[1],
        1, 5, 1.0);

    test_one<Polygon, Polygon, Polygon>("first_within_second2",
        first_within_second[1], first_within_second[0],
        1, 5, 1.0);

    test_one<Polygon, Polygon, Polygon>("first_within_hole_of_second",
            first_within_hole_of_second[0], first_within_hole_of_second[1],
            0, 0, 0.0);

    // Two forming new hole
    test_one<Polygon, Polygon, Polygon>("new_hole",
        new_hole[0], new_hole[1],
        2, 10, 2.0);

    // Two identical
    test_one<Polygon, Polygon, Polygon>("identical",
        identical[0], identical[1],
        1, 5, 1.0);

    test_one<Polygon, Polygon, Polygon>("intersect_exterior_and_interiors_winded",
        intersect_exterior_and_interiors_winded[0], intersect_exterior_and_interiors_winded[1],
        1, 14, 25.2166667);

    test_one<Polygon, Polygon, Polygon>("intersect_holes_disjoint",
        intersect_holes_disjoint[0], intersect_holes_disjoint[1],
        1, 15, 18.0);

    test_one<Polygon, Polygon, Polygon>("intersect_holes_intersect",
        intersect_holes_intersect[0], intersect_holes_intersect[1],
        1, 14, 18.25);

    test_one<Polygon, Polygon, Polygon>("intersect_holes_intersect_and_disjoint",
        intersect_holes_intersect_and_disjoint[0], intersect_holes_intersect_and_disjoint[1],
        1, 19, 17.25);

    test_one<Polygon, Polygon, Polygon>("intersect_holes_intersect_and_touch",
        intersect_holes_intersect_and_touch[0], intersect_holes_intersect_and_touch[1],
        1, 23, 17.25);

    test_one<Polygon, Polygon, Polygon>("intersect_holes_new_ring",
        intersect_holes_new_ring[0], intersect_holes_new_ring[1],
        2, 23, 122.1039);

    test_one<Polygon, Polygon, Polygon>("winded",
        winded[0], winded[1],
        1, 22, 40.0);

    test_one<Polygon, Polygon, Polygon>("within_holes_disjoint",
        within_holes_disjoint[0], within_holes_disjoint[1],
        1, 15, 23.0);

    test_one<Polygon, Polygon, Polygon>("side_side",
        side_side[0], side_side[1],
        0, 0, 0.0);

    test_one<Polygon, Polygon, Polygon>("two_bends",
        two_bends[0], two_bends[1],
        1, 7, 24.0);

    test_one<Polygon, Polygon, Polygon>("star_comb_15",
        star_comb_15[0], star_comb_15[1],
        28, 150, 189.952883);

    test_one<Polygon, Polygon, Polygon>("simplex_normal",
        simplex_normal[0], simplex_normal[1],
        1, 7, 5.47363293);

    test_one<Polygon, Polygon, Polygon>("distance_zero",
        distance_zero[0], distance_zero[1],
        1, 0, 0.29516139);

    test_one<Polygon, Polygon, Polygon>("equal_holes_disjoint",
        equal_holes_disjoint[0], equal_holes_disjoint[1],
        1, 20, 81.0 - 2.0 * 3.0 * 3.0 - 3.0 * 7.0);

    test_one<Polygon, Polygon, Polygon>("only_hole_intersections1",
        only_hole_intersections[0], only_hole_intersections[1],
        1, 21, 178.090909);
    test_one<Polygon, Polygon, Polygon>("only_hole_intersection2",
        only_hole_intersections[0], only_hole_intersections[2],
        1, 21, 149.090909);

    test_one<Polygon, Polygon, Polygon>("fitting",
        fitting[0], fitting[1],
        0, 0, 0.0);

    test_one<Polygon, Polygon, Polygon>("crossed",
        crossed[0], crossed[1],
        3, 0, 1.5);

    test_one<Polygon, Polygon, Polygon>("pie_2_3_23_0",
        pie_2_3_23_0[0], pie_2_3_23_0[1],
        1, 4, 163292.679042133, ut_settings(0.1));

    TEST_INTERSECTION(isovist, 1, 19, expectation_limits(88.19202, 88.19206));

    TEST_INTERSECTION(geos_1, 1, -1, expectation_limits(3454, 3462));

    // Can, in some cases, create small slivers
    // In some cases: 1.430511474609375e-05 (clang/gcc on Xubuntu using b2)
    // In some cases: 5.6022983000000002e-05 (powerpc64le-gcc-6-0)
    TEST_INTERSECTION(geos_2, count_set(0, 1, 2), 0, optional_sliver(6.0e-5));

    TEST_INTERSECTION(geos_3, optional(), 0, optional_sliver(3.0e-7));
    TEST_INTERSECTION(geos_4, 1, -1, expectation_limits(0.08368, 0.08370));

    TEST_INTERSECTION(ggl_list_20110306_javier, 1, -1, expectation_limits(0.6649, 0.6670));

    // SQL Server reports: 0.400390625
    // PostGIS reports 0.4
    // BG did report 0.4 but with rescaling 0.397
    // when selecting other IP closer at endpoint or if segment B is smaller than A
    test_one<Polygon, Polygon, Polygon>("ggl_list_20110307_javier",
        ggl_list_20110307_javier[0], ggl_list_20110307_javier[1],
        1, 4, {0.397162651, 0.40});

    test_one<Polygon, Polygon, Polygon>("ggl_list_20110627_phillip",
        ggl_list_20110627_phillip[0], ggl_list_20110627_phillip[1],
        1, -1, 11151.6618);

    test_one<Polygon, Polygon, Polygon>("ggl_list_20110716_enrico",
        ggl_list_20110716_enrico[0], ggl_list_20110716_enrico[1],
        3, 16, 35723.8506317139);

    test_one<Polygon, Polygon, Polygon>("ggl_list_20131119_james",
        ggl_list_20131119_james[0], ggl_list_20131119_james[1],
        1, 4, 6.6125873045);

    test_one<Polygon, Polygon, Polygon>("ggl_list_20140223_shalabuda",
        ggl_list_20140223_shalabuda[0], ggl_list_20140223_shalabuda[1],
        1, 4, {3.771058, 3.771066});

    // Mailed to the Boost.Geometry list on 2014/03/21 by 7415963@gmail.com
    test_one<Polygon, Polygon, Polygon>("ggl_list_20140321_7415963",
        ggl_list_20140321_7415963[0], ggl_list_20140321_7415963[1],
        0, 0, 0);

    TEST_INTERSECTION(ggl_list_20190307_matthieu_1, 2, -1, 0.035136);
    TEST_INTERSECTION(ggl_list_20190307_matthieu_2, 1, -1, 3.64285);

    test_one<Polygon, Polygon, Polygon>("buffer_rt_f", buffer_rt_f[0], buffer_rt_f[1],
                1, 4, expectation_limits(0.00029437, 0.000294380));
    test_one<Polygon, Polygon, Polygon>("buffer_rt_g", buffer_rt_g[0], buffer_rt_g[1],
                1, 0, 2.914213562373);

    test_one<Polygon, Polygon, Polygon>("ticket_8254", ticket_8254[0], ticket_8254[1],
                optional(), -1, optional_sliver(1e-07));
    test_one<Polygon, Polygon, Polygon>("ticket_6958", ticket_6958[0], ticket_6958[1],
                optional(), -1, optional_sliver());
    test_one<Polygon, Polygon, Polygon>("ticket_8652", ticket_8652[0], ticket_8652[1],
                1, 4, 0.0003);

    TEST_INTERSECTION(ticket_8310a, 1, 5, 0.3843747);
    TEST_INTERSECTION(ticket_8310b, 1, 5, 0.3734379);
    TEST_INTERSECTION(ticket_8310c, 1, 5, 0.4689541);
    TEST_INTERSECTION_REV(ticket_8310a, 1, 5, 0.3843747);
    TEST_INTERSECTION_REV(ticket_8310b, 1, 5, 0.3734379);
    TEST_INTERSECTION_REV(ticket_8310c, 1, 5, 0.4689541);

    test_one<Polygon, Polygon, Polygon>("ticket_9081_15",
                ticket_9081_15[0], ticket_9081_15[1],
                1, 4, 0.0068895780745301394);

    test_one<Polygon, Polygon, Polygon>("ticket_10108_a",
                ticket_10108_a[0], ticket_10108_a[1],
                0, 0, 0.0);

    // msvc  5.6023011e-5
    // mingw 5.6022954e-5
    test_one<Polygon, Polygon, Polygon>("ticket_10108_b",
                ticket_10108_b[0], ticket_10108_b[1],
            optional(), 0, optional_sliver(1.0e-4));

    test_one<Polygon, Polygon, Polygon>("ticket_10747_a",
                ticket_10747_a[0], ticket_10747_a[1],
                1, 4, 70368744177664.0);
    test_one<Polygon, Polygon, Polygon>("ticket_10747_b",
                ticket_10747_b[0], ticket_10747_b[1],
                1, 4, 7036874417766400.0);
    test_one<Polygon, Polygon, Polygon>("ticket_10747_c",
                ticket_10747_c[0], ticket_10747_c[1],
                1, 4, 17592186044416.0);
    test_one<Polygon, Polygon, Polygon>("ticket_10747_d",
                ticket_10747_d[0], ticket_10747_d[1],
                1, 4, 703687777321.0);

    // Delivers very small triangle < 1.0e-13, or zero
    test_one<Polygon, Polygon, Polygon>("ticket_10747_e",
                ticket_10747_e[0], ticket_10747_e[1],
                optional(), -1, optional_sliver(1.0e-13));

    test_one<Polygon, Polygon, Polygon>("ticket_11576",
                ticket_11576[0], ticket_11576[1],
                1, -1, expectation_limits(5.5856173e-07, 5.5856175e-07));

    {
        // Not yet valid when rescaling is turned off
        ut_settings settings;
        settings.set_test_validity(false);
        test_one<Polygon, Polygon, Polygon>("ticket_9563", ticket_9563[0], ticket_9563[1],
                    1, 8, 129.90381, settings);
    }

    TEST_INTERSECTION(issue_548, 1, -1, expectation_limits(1958821942, 1958824416));

    TEST_INTERSECTION(issue_566_a, 1, -1, 70.7107);
    TEST_INTERSECTION(issue_566_b, 1, -1, 70.7107);

    TEST_INTERSECTION(issue_838, 1, -1, (expectation_limits{0.6582, 0.6650}));

    TEST_INTERSECTION(issue_861, 1, -1, 1.4715007684573677693e-10);

    TEST_INTERSECTION(issue_893, 1, -1, 473001.5082956461);

    TEST_INTERSECTION(issue_1229, 0, -1, 0);

    test_one<Polygon, Polygon, Polygon>("buffer_mp1", buffer_mp1[0], buffer_mp1[1],
                1, 31, 2.271707796);
    test_one<Polygon, Polygon, Polygon>("buffer_mp2", buffer_mp2[0], buffer_mp2[1],
                1, 29, 0.457126);

    test_one<Polygon, Polygon, Polygon>("case_58_iet",
        case_58[0], case_58[2],
        2, -1, 1.0 / 3.0);

    test_one<Polygon, Polygon, Polygon>("case_80",
        case_80[0], case_80[1],
        0, 0, 0.0);

    test_one<Polygon, Polygon, Polygon>("case_81",
        case_81[0], case_81[1],
        0, 0, 0.0);

    test_one<Polygon, Polygon, Polygon>("case_101",
        case_101[0], case_101[1],
        1, -1, 6.25);
    test_one<Polygon, Polygon, Polygon>("case_102",
        case_102[0], case_102[1],
        count_set(1, 2), -1, 3.1875);

    test_one<Polygon, Polygon, Polygon>("case_103",
        case_103[0], case_103[1],
        1, -1, 0.5);
    test_one<Polygon, Polygon, Polygon>("case_104",
        case_104[0], case_104[1],
        0, -1, 0.0);

    TEST_INTERSECTION(case_105, 1, 34, 76.0);
    TEST_INTERSECTION(case_106, 2, -1, 3.5);
    TEST_INTERSECTION(case_107, 3, -1, 3.0);

    TEST_INTERSECTION(case_precision_1, optional(), 0, optional_sliver(1.0e-4));
    TEST_INTERSECTION(case_precision_2, optional(), 0, optional_sliver(1.0e-5));
    TEST_INTERSECTION(case_precision_3, optional(), 0, optional_sliver(1.0e-7));
    TEST_INTERSECTION(case_precision_4, 0, 0, 0.0);
    TEST_INTERSECTION(case_precision_5, optional(), 0, optional_sliver(1.0e-6));
    TEST_INTERSECTION(case_precision_6, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_7, 0, -1, 0.0);
    TEST_INTERSECTION(case_precision_8, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_9, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_10, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_11, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_12, 1, -1, 2.0);
    TEST_INTERSECTION(case_precision_13, 1, -1, 1.99998);
    TEST_INTERSECTION(case_precision_14, 0, -1, 0.0);
    TEST_INTERSECTION(case_precision_15, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_16, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_17, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_18, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_19, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_20, 0, 0, 0.0);
    TEST_INTERSECTION(case_precision_21, 0, 0, 0.0);
    TEST_INTERSECTION(case_precision_22, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_23, 1, -1, 14.0);
    TEST_INTERSECTION(case_precision_24, 0, 0, 0.0);
    TEST_INTERSECTION(case_precision_25, 0, 0, 0.0);
    TEST_INTERSECTION(case_precision_26, 1, -1, 14.0);

    TEST_INTERSECTION_REV(case_precision_1, optional(), 0, optional_sliver(1.0e-4));
    TEST_INTERSECTION_REV(case_precision_2, optional(), 0, optional_sliver(1.0e-5));
    TEST_INTERSECTION_REV(case_precision_3, optional(), 0, optional_sliver(1.0e-7));
    TEST_INTERSECTION_REV(case_precision_4, 0, 0, 0.0);
    TEST_INTERSECTION_REV(case_precision_5, optional(), 0, optional_sliver(1.0e-6));
    TEST_INTERSECTION_REV(case_precision_6, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_7, 0, -1, 0.0);
    TEST_INTERSECTION_REV(case_precision_8, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_9, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_10, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_11, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_12, 1, -1, 2.0);
    TEST_INTERSECTION_REV(case_precision_13, 1, -1, 1.99998);
    TEST_INTERSECTION_REV(case_precision_14, 0, -1, 0.0);
    TEST_INTERSECTION_REV(case_precision_15, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_16, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_17, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_18, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_19, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_20, 0, 0, 0.0);
    TEST_INTERSECTION_REV(case_precision_21, 0, 0, 0.0);
    TEST_INTERSECTION_REV(case_precision_22, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_23, 1, -1, 14.0);
    TEST_INTERSECTION_REV(case_precision_24, 0, 0, 0.0);
    TEST_INTERSECTION_REV(case_precision_25, 0, 0, 0.0);
    TEST_INTERSECTION_REV(case_precision_26, 1, -1, 14.0);

    test_one<Polygon, Polygon, Polygon>("mysql_21964049",
        mysql_21964049[0], mysql_21964049[1],
        0, -1, 0.0);

    test_one<Polygon, Polygon, Polygon>("mysql_21964465",
        mysql_21964465[0], mysql_21964465[1],
        0, -1, 0.0);

    test_one<Polygon, Polygon, Polygon>("mysql_21965285_b_inv",
        mysql_21965285_b_inv[0],
        mysql_21965285_b_inv[1],
        2, -1, 183.71376870369406);

    TEST_INTERSECTION(mysql_23023665_6, 2, 0, 11.812440191387557);

    // Formation of an interior ring is optional for these cases
    TEST_INTERSECTION(mysql_23023665_10, optional(), 1, 54.701340543162523);
    TEST_INTERSECTION(mysql_23023665_11, optional(), 1, 35.933385462482065);

//    test_one<Polygon, Polygon, Polygon>(
//        "polygon_pseudo_line",
//        "Polygon((0 0,0 4,4 4,4 0,0 0))",
//        "Polygon((2 -2,2 -1,2 6,2 -2))",
//        5, 22, 1.1901714);
}

template <typename Polygon, typename Box>
void test_areal_clip()
{
    test_one<Polygon, Box, Polygon>("boxring", example_box, example_ring,
        2, 12, 1.09125);
    test_one<Polygon, Polygon, Box>("boxring2", example_ring,example_box,
        2, 12, 1.09125);

    test_one<Polygon, Box, Polygon>("boxpoly", example_box, example_polygon,
        3, 19, 0.840166);

    test_one<Polygon, Box, Polygon>("poly1", example_box,
        "POLYGON((3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2))",
        2, 12, 1.09125);

    test_one<Polygon, Box, Polygon>("clip_poly2", example_box,
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,5.3 2.5,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))",
        2, 12, 1.00375);

    test_one<Polygon, Box, Polygon>("clip_poly3", example_box,
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 2.5,4.5 1.2,4.9 0.8,2.9 0.7,2 1.3))",
        2, 12, 1.00375);

    test_one<Polygon, Box, Polygon>("clip_poly4", example_box,
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 2.5,4.5 2.3,5.0 2.3,5.0 2.1,4.5 2.1,4.5 1.9,4.0 1.9,4.5 1.2,4.9 0.8,2.9 0.7,2 1.3))",
        2, 16, 0.860892);

    test_one<Polygon, Box, Polygon>("clip_poly5", example_box,
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 2.5,4.5 1.2,2.9 0.7,2 1.3))",
        2, 11, 0.7575961);

    test_one<Polygon, Box, Polygon>("clip_poly6", example_box,
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.0 3.0,5.0 2.0,2.9 0.7,2 1.3))",
        2, 13, 1.0744456);

    test_one<Polygon, Box, Polygon>("clip_poly7", "Box(0 0, 3 3)",
        "POLYGON((2 2, 1 4, 2 4, 3 3, 2 2))",
        1, 4, 0.75);
}


template <typename P>
void test_point_output()
{
    typedef bg::model::linestring<P> linestring;
    typedef bg::model::polygon<P> polygon;
    typedef bg::model::box<P> box;
    //typedef bg::model::segment<P> segment;

    test_point_output<polygon, polygon>(simplex_normal[0], simplex_normal[1], 6);
    test_point_output<box, polygon>("box(1 1,6 4)", simplex_normal[0], 4);
    test_point_output<linestring, polygon>("linestring(0 2,6 2)", simplex_normal[0], 2);
    // NYI because of sectionize:
    // test_point_output<segment, polygon>("linestring(0 2,6 2)", simplex_normal[0], 2);
    // NYI because needs special treatment:
    // test_point_output<box, box>("box(0 0,4 4)", "box(2 2,6 6)", 2);
}


template <typename Polygon, typename LineString>
void test_areal_linear()
{
    std::string const poly_simplex = "POLYGON((1 1,1 3,3 3,3 1,1 1))";

    test_one_lp<LineString, Polygon, LineString>("simplex", poly_simplex, "LINESTRING(0 2,4 2)", 1, 2, 2.0);
    test_one_lp<LineString, Polygon, LineString>("case2",   poly_simplex, "LINESTRING(0 1,4 3)", 1, 2, sqrt(5.0));
    test_one_lp<LineString, Polygon, LineString>("case3", "POLYGON((2 0,2 5,5 5,5 0,2 0))", "LINESTRING(0 1,1 2,3 2,4 3,6 3,7 4)", 1, 4, 2 + sqrt(2.0));
    test_one_lp<LineString, Polygon, LineString>("case4", "POLYGON((0 0,0 4,2 4,2 0,0 0))", "LINESTRING(1 1,3 2,1 3)", 2, 4, sqrt(5.0));

    test_one_lp<LineString, Polygon, LineString>("case5", poly_simplex, "LINESTRING(0 1,3 4)", 1, 2, sqrt(2.0));
    test_one_lp<LineString, Polygon, LineString>("case6", "POLYGON((2 0,2 4,3 4,3 1,4 1,4 3,5 3,5 1,6 1,6 3,7 3,7 1,8 1,8 3,9 3,9 0,2 0))", "LINESTRING(1 1,10 3)", 4, 8,
            // Pieces are 1 x 2/9:
            4.0 * sqrt(1.0 + 4.0/81.0));
    test_one_lp<LineString, Polygon, LineString>("case7", poly_simplex, "LINESTRING(1.5 1.5,2.5 2.5)", 1, 2, sqrt(2.0));
    test_one_lp<LineString, Polygon, LineString>("case8", poly_simplex, "LINESTRING(1 0,2 0)", 0, 0, 0.0);

    std::string const poly_9 = "POLYGON((1 1,1 4,4 4,4 1,1 1))";
    test_one_lp<LineString, Polygon, LineString>("case9", poly_9, "LINESTRING(0 1,1 2,2 2)", 1, 2, 1.0);
    test_one_lp<LineString, Polygon, LineString>("case10", poly_9, "LINESTRING(0 1,1 2,0 2)", 0, 0, 0.0);
    test_one_lp<LineString, Polygon, LineString>("case11", poly_9, "LINESTRING(2 2,4 2,3 3)", 1, 3, 2.0 + sqrt(2.0));
    test_one_lp<LineString, Polygon, LineString>("case12", poly_9, "LINESTRING(2 3,4 4,5 6)", 1, 2, sqrt(5.0));

    test_one_lp<LineString, Polygon, LineString>("case13", poly_9, "LINESTRING(3 2,4 4,2 3)", 1, 3, 2.0 * sqrt(5.0));
    test_one_lp<LineString, Polygon, LineString>("case14", poly_9, "LINESTRING(5 6,4 4,6 5)", 0, 0, 0.0);
    test_one_lp<LineString, Polygon, LineString>("case15", poly_9, "LINESTRING(0 2,1 2,1 3,0 3)", 1, 2, 1.0);
    test_one_lp<LineString, Polygon, LineString>("case16", poly_9, "LINESTRING(2 2,1 2,1 3,2 3)", 1, 4, 3.0);

    std::string const angly = "LINESTRING(2 2,2 1,4 1,4 2,5 2,5 3,4 3,4 4,5 4,3 6,3 5,2 5,2 6,0 4)";
    // PROPERTIES CHANGED BY switch_to_integer
    // TODO test_one_lp<LineString, Polygon, LineString>("case17", "POLYGON((1 1,1 5,4 5,4 1,1 1))", angly, 3, 8, 6.0);
    test_one_lp<LineString, Polygon, LineString>("case18", "POLYGON((1 1,1 5,5 5,5 1,1 1))", angly, 2, 12, 10.0 + sqrt(2.0));
    test_one_lp<LineString, Polygon, LineString>("case19", poly_9, "LINESTRING(1 2,1 3,0 3)", 1, 2, 1.0);
    test_one_lp<LineString, Polygon, LineString>("case20", poly_9, "LINESTRING(1 2,1 3,2 3)", 1, 3, 2.0);

    test_one_lp<LineString, Polygon, LineString>("case21",
        "POLYGON((2 3,-9 -7,12 -13,2 3))",
        "LINESTRING(-1.3 0,-15 0,-1.3 0)",
         0, 0, 0);

    test_one_lp<LineString, Polygon, LineString>("case22",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(5 5,-10 5,5 5)",
         2, 4, 10);

    test_one_lp<LineString, Polygon, LineString>("case22a",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(1 1,5 5,-10 5,5 5,6 6)",
         2, 6, 17.071068);

    test_one_lp<LineString, Polygon, LineString>("case23",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-10 5,5 5,-10 5)",
         1, 3, 10);

    test_one_lp<LineString, Polygon, LineString>("case23a",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-20 10,-10 5,5 5,-10 5,-20 -10)",
         1, 3, 10);

    test_one_lp<LineString, Polygon, LineString>("case24",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(0 5,5 5,0 5)",
         1, 3, 10);

    test_one_lp<LineString, Polygon, LineString>("case24",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(0 5,5 5,1 1,9 1,5 5,0 5)",
         1, 6, 29.313708);

    test_one_lp<LineString, Polygon, LineString>("case25",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(5 5,0 5,5 5)",
         1, 3, 10);

    test_one_lp<LineString, Polygon, LineString>("case25a",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-10 10,5 5,0 5,5 5,20 10)",
         1, 4, 20.540925);

    test_one_lp<LineString, Polygon, LineString>("case25b",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-10 10,5 5,1 5,5 5,20 10)",
         1, 4, 18.540925);

    test_one_lp<LineString, Polygon, LineString>("case25c",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-10 10,5 5,-1 5,5 5,20 10)",
         2, 6, 20.540925);

    test_one_lp<LineString, Polygon, LineString>("case26",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-5 5,0 5,-5 5)",
         0, 0, 0);

    test_one_lp<LineString, Polygon, LineString>("case26a",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(-10 10,-5 5,0 5,-5 5,-10 -10)",
         0, 0, 0);

    test_one_lp<LineString, Polygon, LineString>("case27",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(5 5,0 5,5 5,5 4,0 4,5 4)",
         1, 6, 21.0);

    test_one_lp<LineString, Polygon, LineString>("case28",
        "POLYGON((0 0,0 10,10 10,10 0,0 0))",
        "LINESTRING(5 5,0 5,5 5,5 4,0 4,5 3)",
         1, 6, 21.099019);

    test_one_lp<LineString, Polygon, LineString>("case29",
        "POLYGON((5 5,15 15,15 5,5 5))",
        "LINESTRING(0 0,10 10)",
        1, 2, 5 * std::sqrt(2.0));

    // PROPERTIES CHANGED BY switch_to_integer
    // TODO test_one_lp<LineString, Polygon, LineString>("case21", poly_9, "LINESTRING(1 2,1 4,4 4,4 1,2 1,2 2)", 1, 6, 11.0);

    // Compile test - arguments in any order:
    test_one<LineString, Polygon, LineString>("simplex", poly_simplex, "LINESTRING(0 2,4 2)", 1, 2, 2.0);
    test_one<LineString, LineString, Polygon>("simplex", "LINESTRING(0 2,4 2)", poly_simplex, 1, 2, 2.0);

    typedef typename bg::point_type<Polygon>::type Point;
    test_one<LineString, bg::model::ring<Point>, LineString>("simplex", poly_simplex, "LINESTRING(0 2,4 2)", 1, 2, 2.0);

    test_one_lp<LineString, Polygon, LineString>("case30",
        "POLYGON((25 0,0 15,30 15,22 10,25 0))",
        "LINESTRING(10 15,20 15)",
        1, 2, 10.0);

    test_one_lp<LineString, Polygon, LineString>("case31",
        "POLYGON((25 0,0 15,30 15,22 10,25 0))",
        "LINESTRING(0 15,20 15)",
        1, 2, 20.0);

    test_one_lp<LineString, Polygon, LineString>("case32",
        "POLYGON((25 0,0 15,30 15,22 10,25 0))",
        "LINESTRING(25 0, 0 15,20 15)",
        1, 3, 49.15475947422650 /*sqrt(25^2+15^2)+20*/);

    typedef typename bg::point_type<Polygon>::type P;

    test_one_lp<P, Polygon, LineString>("case30p",
        "POLYGON((25 0,0 15,30 15,22 10,25 0))",
        "LINESTRING(10 15,20 15)",
        2, 2, 0);
}


template <typename Linestring, typename Box>
void test_linear_box()
{
    typedef bg::model::multi_linestring<Linestring> multi_linestring_type;

    test_one_lp<Linestring, Box, Linestring>
        ("case-l-b-01",
         "BOX(-10 -10,10 10)",
         "LINESTRING(-20 -20, 0 0,20 20)",
         1, 3, 20 * sqrt(2.0));

    test_one_lp<Linestring, Box, Linestring>
        ("case-l-b-02",
         "BOX(-10 -10,10 10)",
         "LINESTRING(-20 -20, 20 20)",
         1, 2, 20.0 * sqrt(2.0));

    test_one_lp<Linestring, Box, Linestring>
        ("case-l-b-02",
         "BOX(-10 -10,10 10)",
         "LINESTRING(-20 -20, 20 20,15 0,0 -15)",
         2, 4, 25.0 * sqrt(2.0));

    test_one_lp<Linestring, Box, multi_linestring_type>
        ("case-ml-b-01",
         "BOX(-10 -10,10 10)",
         "MULTILINESTRING((-20 -20, 20 20),(0 -15,15 0))",
         2, 4, 25.0 * sqrt(2.0));
}


template <typename P>
void test_all()
{
    typedef bg::model::linestring<P> linestring;
    typedef bg::model::polygon<P> polygon;
    typedef bg::model::box<P> box;
    typedef bg::model::segment<P> segment;

    typedef bg::model::polygon<P, false> polygon_ccw;
    typedef bg::model::polygon<P, true, false> polygon_open;
    typedef bg::model::polygon<P, false, false> polygon_ccw_open;
    boost::ignore_unused<polygon_ccw, polygon_open, polygon_ccw_open>();

    std::string clip = "box(2 2,8 8)";

    test_areal_linear<polygon, linestring>();
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_ORDER)
    test_areal_linear<polygon_ccw, linestring>();
#endif
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    test_areal_linear<polygon_open, linestring>();
    test_areal_linear<polygon_ccw_open, linestring>();
#endif

    test_linear_box<linestring, box>();

    // Test polygons clockwise and counter clockwise
    test_areal<polygon>();

#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_ORDER)
    test_areal<polygon_ccw>();
#endif
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    test_areal<polygon_open>();
    test_areal<polygon_ccw_open>();
#endif

    test_areal_clip<polygon, box>();
#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    test_areal_clip<polygon_ccw, box>();
#endif

#if defined(TEST_FAIL_DIFFERENT_ORIENTATIONS)
    // Should NOT compile
    // NOTE: this can probably be relaxed later on.
    test_one<polygon, polygon_ccw, polygon>("simplex_normal",
        simplex_normal[0], simplex_normal[1],
        1, 7, 5.47363293);
    // Output ccw, nyi (should be just reversing afterwards)
    test_one<polygon, polygon, polygon_ccw>("simplex_normal",
        simplex_normal[0], simplex_normal[1],
        1, 7, 5.47363293);
#endif

    // Basic check: box/linestring, is clipping OK? should compile in any order
    test_one<linestring, linestring, box>("llb", "LINESTRING(0 0,10 10)", clip, 1, 2, sqrt(2.0 * 6.0 * 6.0));
    test_one<linestring, box, linestring>("lbl", clip, "LINESTRING(0 0,10 10)", 1, 2, sqrt(2.0 * 6.0 * 6.0));

    // Box/segment
    test_one<linestring, segment, box>("lsb", "LINESTRING(0 0,10 10)", clip, 1, 2, sqrt(2.0 * 6.0 * 6.0));
    test_one<linestring, box, segment>("lbs", clip, "LINESTRING(0 0,10 10)", 1, 2, sqrt(2.0 * 6.0 * 6.0));

    // Completely inside
    test_one<linestring, linestring, box>("llbi", "LINESTRING(3 3,7 7)", clip, 1, 2, sqrt(2.0 * 4.0 * 4.0));

    // Completely outside
    test_one<linestring, linestring, box>("llbo", "LINESTRING(9 9,10 10)", clip, 0, 0, 0.0);

    // Touching with point (-> output linestring with ONE point)
    test_one<linestring, linestring, box>("llb_touch", "LINESTRING(8 8,10 10)", clip, 1, 1, 0.0,
                                          ut_settings(0.0001, false));

    // Along border
    test_one<linestring, linestring, box>("llb_along", "LINESTRING(2 2,2 8)", clip, 1, 2, 6.0);

    // Outputting two lines (because of 3-4-5 constructions (0.3,0.4,0.5)
    // which occur 4 times, the length is expected to be 2.0)
    test_one<linestring, linestring, box>("llb_2", "LINESTRING(1.7 1.6,2.3 2.4,2.9 1.6,3.5 2.4,4.1 1.6)", clip, 2, 6, 4.0 * 0.5);

    // linear
    test_one<P, linestring, linestring>("llp1", "LINESTRING(0 0,1 1)", "LINESTRING(0 1,1 0)", 1, 1, 0.0);
    test_one<P, segment, segment>("ssp1", "LINESTRING(0 0,1 1)", "LINESTRING(0 1,1 0)", 1, 1, 0.0);
    test_one<P, linestring, linestring>("llp2", "LINESTRING(0 0,1 1)", "LINESTRING(0 0,2 2)", 2, 2, 0.0);

    // polygons outputing points
    //test_one<P, polygon, polygon>("ppp1", simplex_normal[0], simplex_normal[1], 1, 7, 5.47363293);

    test_point_output<P>();

    /*
    test_one<polygon, box, polygon>(99, "box(115041.10 471900.10, 118334.60 474523.40)",
            "POLYGON ((115483.40 474533.40, 116549.40 474059.20, 117199.90 473762.50, 117204.90 473659.50, 118339.40 472796.90, 118334.50 472757.90, 118315.10 472604.00, 118344.60 472520.90, 118277.90 472419.10, 118071.40 472536.80, 118071.40 472536.80, 117943.10 472287.70, 117744.90 472248.40, 117708.00 472034.50, 117481.90 472056.90, 117481.90 472056.90, 117272.30 471890.10, 117077.90 472161.20, 116146.60 473054.50, 115031.10 473603.30, 115483.40 474533.40))",
                1, 26, 3727690.74);
    */
}

void test_pointer_version()
{
    std::vector<test::test_point_xy*> ln;
    test::test_point_xy* p;
    p = new test::test_point_xy; p->x = 0; p->y = 0; ln.push_back(p);
    p = new test::test_point_xy; p->x = 10; p->y = 10; ln.push_back(p);

    bg::model::box<bg::model::d2::point_xy<double> > box;
    bg::assign_values(box, 2, 2, 8, 8);

    typedef bg::model::linestring<bg::model::d2::point_xy<double> > output_type;
    std::vector<output_type> clip;
    bg::detail::intersection::intersection_insert<output_type>(box, ln, std::back_inserter(clip));

    double length = 0;
    std::size_t n = 0;
    for (std::vector<output_type>::const_iterator it = clip.begin();
            it != clip.end(); ++it)
    {
        length += bg::length(*it);
        n += bg::num_points(*it);
    }

    BOOST_CHECK_EQUAL(clip.size(), 1u);
    BOOST_CHECK_EQUAL(n, 2u);
    BOOST_CHECK_CLOSE(length, sqrt(2.0 * 6.0 * 6.0), 0.001);

    for (std::size_t i = 0; i < ln.size(); i++)
    {
        delete ln[i];
    }
}


template <typename P>
void test_exception()
{
    typedef bg::model::polygon<P> polygon;

    try
    {
        // Define polygon with a spike (= invalid)
        std::string spike = "POLYGON((0 0,0 4,2 4,2 6,2 4,4 4,4 0,0 0))";

        test_one<polygon, polygon, polygon>("with_spike",
            simplex_normal[0], spike,
            0, 0, 0);
    }
    catch(bg::overlay_invalid_input_exception const& )
    {
        return;
    }
    BOOST_CHECK_MESSAGE(false, "No exception thrown");
}

template <typename Point>
void test_rational()
{
    typedef bg::model::polygon<Point> polygon;
    test_one<polygon, polygon, polygon>("simplex_normal",
        simplex_normal[0], simplex_normal[1],
        1, 7, 5.47363293);
}

int test_main(int, char* [])
{
    BoostGeometryWriteTestConfiguration();
    test_all<bg::model::d2::point_xy<default_test_type> >();

#if ! defined(BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE)
    test_all<bg::model::d2::point_xy<float> >();

    // Commented, because exception is now disabled:
    // test_exception<bg::model::d2::point_xy<double> >();

    test_pointer_version();
    test_rational<bg::model::d2::point_xy<boost::rational<int> > >();
#endif

    return 0;
}
