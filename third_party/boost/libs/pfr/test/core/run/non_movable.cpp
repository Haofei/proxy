// Copyright (c) 2018-2025 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/pfr/tuple_size.hpp>

#include <boost/core/lightweight_test.hpp>

struct X {
    X() = default;
    X(X&&) = delete;
    X(const X&) = delete;

    X& operator=(X&&) = delete;
    X& operator=(const X&) = delete;
};
struct S { X x0; X x1; int x2; X x3; };

int main() {

#if BOOST_PFR_HAS_GUARANTEED_COPY_ELISION

// MSVC-14.3 fails this test
#if !defined(_MSC_VER) || _MSC_VER < 1930 || _MSC_VER > 1949
    static_assert(boost::pfr::tuple_size_v<S> == 4, "");

    struct S5_0 { int x0; int x1; int x2; int x3; X x4; };
    static_assert(boost::pfr::tuple_size_v<S5_0> == 5, "");

    struct S5_1 { X x0; int x1; int x2; int x3; int x4; };
    static_assert(boost::pfr::tuple_size_v<S5_1> == 5, "");

    struct S5_2 { int x0; int x1; X x2; int x3; int x4; };
    static_assert(boost::pfr::tuple_size_v<S5_2> == 5, "");

    struct S5_3 { int x0; int x1; X x2; int x3; X x4; };
    static_assert(boost::pfr::tuple_size_v<S5_3> == 5, "");

    struct S5_4 { X x0; X x1; X x2; X x3; X x4; };
    static_assert(boost::pfr::tuple_size_v<S5_4> == 5, "");

    struct S6 { X x0; X x1; X x2; X x3; X x4;  X x5;};
    static_assert(boost::pfr::tuple_size_v<S6> == 6, "");
#endif

#endif  // #if BOOST_PFR_HAS_GUARANTEED_COPY_ELISION

    return boost::report_errors();
}

