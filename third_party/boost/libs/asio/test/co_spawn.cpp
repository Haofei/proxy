//
// co_spawn.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/asio/co_spawn.hpp>

#include "unit_test.hpp"

#if defined(BOOST_ASIO_HAS_CO_AWAIT)

#include <stdexcept>
#include <boost/asio/any_completion_handler.hpp>
#include <boost/asio/bind_cancellation_slot.hpp>
#include <boost/asio/io_context.hpp>

boost::asio::awaitable<void> void_returning_coroutine()
{
  co_return;
}

boost::asio::awaitable<int> int_returning_coroutine()
{
  co_return 42;
}

void test_co_spawn_with_any_completion_handler()
{
  boost::asio::io_context ctx;

  bool called = false;
  boost::asio::co_spawn(ctx, void_returning_coroutine(),
      boost::asio::any_completion_handler<void(std::exception_ptr)>(
        [&](std::exception_ptr)
        {
          called = true;
        }));

  BOOST_ASIO_CHECK(!called);

  ctx.run();

  BOOST_ASIO_CHECK(called);

  int result = 0;
  boost::asio::co_spawn(ctx, int_returning_coroutine(),
      boost::asio::any_completion_handler<void(std::exception_ptr, int)>(
        [&](std::exception_ptr, int i)
        {
          result = i;
        }));

  BOOST_ASIO_CHECK(result == 0);

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(result == 42);
}

void test_co_spawn_immediate_cancel()
{
  boost::asio::cancellation_signal sig;
  boost::asio::io_context ctx;

  std::exception_ptr result = nullptr;
  bool called = false;
  boost::asio::co_spawn(ctx, void_returning_coroutine(),
      boost::asio::bind_cancellation_slot(sig.slot(),
        [&](std::exception_ptr e)
        {
          result = e;
          called = true;
        }));

  BOOST_ASIO_CHECK(!called);
  BOOST_ASIO_CHECK(result == nullptr);

  sig.emit(boost::asio::cancellation_type::all);
  ctx.run();

  BOOST_ASIO_CHECK(called);
  BOOST_ASIO_CHECK(result != nullptr);

  result = nullptr;
  called = false;
  boost::asio::co_spawn(ctx, int_returning_coroutine(),
      boost::asio::bind_cancellation_slot(sig.slot(),
        [&](std::exception_ptr e, int i)
        {
          BOOST_ASIO_CHECK(i != 42);
          result = e;
          called = true;
        }));

  BOOST_ASIO_CHECK(!called);
  BOOST_ASIO_CHECK(result == nullptr);

  sig.emit(boost::asio::cancellation_type::all);
  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(called);
  BOOST_ASIO_CHECK(result != nullptr);
}

BOOST_ASIO_TEST_SUITE
(
  "co_spawn",
  BOOST_ASIO_TEST_CASE(test_co_spawn_with_any_completion_handler)
  BOOST_ASIO_TEST_CASE(test_co_spawn_immediate_cancel)
)

#else // defined(BOOST_ASIO_HAS_CO_AWAIT)

BOOST_ASIO_TEST_SUITE
(
  "co_spawn",
  BOOST_ASIO_TEST_CASE(null_test)
)

#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
