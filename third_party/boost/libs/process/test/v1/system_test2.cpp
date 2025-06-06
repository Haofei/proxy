// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MAIN
#define BOOST_TEST_IGNORE_SIGCHLD
#include <boost/test/included/unit_test.hpp>

#include <boost/system/error_code.hpp>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/steady_timer.hpp>

#include <boost/process/v1/error.hpp>
#include <boost/process/v1/io.hpp>
#include <boost/process/v1/args.hpp>
#include <boost/process/v1/system.hpp>
#include <boost/process/v1/async_pipe.hpp>
#include <boost/process/v1/async.hpp>
#include <system_error>

#include <boost/process/v1/filesystem.hpp>

#include <atomic>
#include <string>
#include <chrono>
#include <istream>
#include <cstdlib>

namespace fs = boost::process::v1::filesystem;
namespace bp = boost::process::v1;

BOOST_AUTO_TEST_CASE(explicit_async_io, *boost::unit_test::timeout(2))
{
    using boost::unit_test::framework::master_test_suite;

    boost::asio::io_context ios;

    std::future<std::string> fut;

    std::error_code ec;
    bp::system(
        master_test_suite().argv[1],
        "test", "--echo-stdout", "abc",
        bp::std_out > fut,
        ios,
        ec
    );
    BOOST_REQUIRE(!ec);

    BOOST_REQUIRE(fut.valid());
    BOOST_REQUIRE(boost::starts_with(fut.get(), "abc"));
}

BOOST_AUTO_TEST_CASE(explicit_async_io_running, *boost::unit_test::timeout(10))
{
    using boost::unit_test::framework::master_test_suite;

    boost::asio::io_context ios;
    std::future<std::string> fut;
    std::error_code ec;

    boost::asio::post(
        ios.get_executor(),
        [&] {
            bp::system(
                master_test_suite().argv[1],
                "test", "--echo-stdout", "abc",
                bp::std_out > fut,
                ios,
                ec
            );
            BOOST_REQUIRE(!ec);
            }
        );


    ios.run();

    BOOST_REQUIRE(fut.valid());
    BOOST_REQUIRE(boost::starts_with(
            fut.get(), "abc"));


}
