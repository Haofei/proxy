//
// cpp14/can_query_not_applicable_static.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio/query.hpp>
#include <cassert>

struct prop
{
  template <typename> static constexpr int static_query_v = 123;
};

struct object
{
};

int main()
{
  static_assert(!boost::asio::can_query_v<object, prop>, "");
  static_assert(!boost::asio::can_query_v<const object, prop>, "");
}
