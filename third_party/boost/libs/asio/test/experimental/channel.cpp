//
// experimental/channel.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
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
#include <boost/asio/experimental/channel.hpp>

#include <utility>
#include <boost/asio/any_completion_handler.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/bind_immediate_executor.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/system_executor.hpp>
#include "../unit_test.hpp"

using namespace boost::asio;
using namespace boost::asio::experimental;

void unbuffered_channel_test()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  BOOST_ASIO_CHECK(ch1.is_open());
  BOOST_ASIO_CHECK(!ch1.ready());

  bool b1 = ch1.try_send(boost::asio::error::eof, "hello");

  BOOST_ASIO_CHECK(!b1);

  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  bool b2 = ch1.try_send(boost::asio::error::eof, std::move(s1));

  BOOST_ASIO_CHECK(!b2);
  BOOST_ASIO_CHECK(!s1.empty());

  boost::system::error_code ec1;
  std::string s2;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec1 = ec;
        s2 = std::move(s);
      });

  bool b3 = ch1.try_send(boost::asio::error::eof, std::move(s1));

  BOOST_ASIO_CHECK(b3);
  BOOST_ASIO_CHECK(s1.empty());

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "abcdefghijklmnopqrstuvwxyz");

  bool b4 = ch1.try_receive([](boost::system::error_code, std::string){});

  BOOST_ASIO_CHECK(!b4);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s3 = "zyxwvutsrqponmlkjihgfedcba";
  ch1.async_send(boost::asio::error::eof, std::move(s3),
      [&](boost::system::error_code ec)
      {
        ec2 = ec;
      });

  boost::system::error_code ec3;
  std::string s4;
  bool b5 = ch1.try_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec3 = ec;
        s4 = s;
      });

  BOOST_ASIO_CHECK(b5);
  BOOST_ASIO_CHECK(ec3 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s4 == "zyxwvutsrqponmlkjihgfedcba");

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(!ec2);
}

void buffered_channel_test()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  BOOST_ASIO_CHECK(ch1.is_open());
  BOOST_ASIO_CHECK(!ch1.ready());

  bool b1 = ch1.try_send(boost::asio::error::eof, "hello");

  BOOST_ASIO_CHECK(b1);

  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  bool b2 = ch1.try_send(boost::asio::error::eof, std::move(s1));

  BOOST_ASIO_CHECK(!b2);
  BOOST_ASIO_CHECK(!s1.empty());

  boost::system::error_code ec1;
  std::string s2;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec1 = ec;
        s2 = std::move(s);
      });

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "hello");

  bool b4 = ch1.try_receive([](boost::system::error_code, std::string){});

  BOOST_ASIO_CHECK(!b4);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s3 = "zyxwvutsrqponmlkjihgfedcba";
  ch1.async_send(boost::asio::error::eof, std::move(s3),
      [&](boost::system::error_code ec)
      {
        ec2 = ec;
      });

  boost::system::error_code ec3;
  std::string s4;
  bool b5 = ch1.try_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec3 = ec;
        s4 = s;
      });

  BOOST_ASIO_CHECK(b5);
  BOOST_ASIO_CHECK(ec3 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s4 == "zyxwvutsrqponmlkjihgfedcba");

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(!ec2);

  bool b6 = ch1.try_send(boost::system::error_code(), "goodbye");

  BOOST_ASIO_CHECK(b6);

  ch1.close();

  boost::system::error_code ec4;
  std::string s5;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec4 = ec;
        s5 = std::move(s);
      });

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(!ec4);
  BOOST_ASIO_CHECK(s5 == "goodbye");

  boost::system::error_code ec5;
  std::string s6;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec5 = ec;
        s6 = std::move(s);
      });

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(ec5 == boost::asio::experimental::channel_errc::channel_closed);
  BOOST_ASIO_CHECK(s6.empty());
}

void buffered_error_channel_test()
{
  io_context ctx;

  channel<void(boost::system::error_code)> ch1(ctx, 1);

  BOOST_ASIO_CHECK(ch1.is_open());
  BOOST_ASIO_CHECK(!ch1.ready());

  bool b1 = ch1.try_send(boost::asio::error::eof);

  BOOST_ASIO_CHECK(b1);

  bool b2 = ch1.try_send(boost::asio::error::eof);

  BOOST_ASIO_CHECK(!b2);

  boost::system::error_code ec1;
  ch1.async_receive(
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);

  bool b4 = ch1.try_receive([](boost::system::error_code){});

  BOOST_ASIO_CHECK(!b4);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  ch1.async_send(boost::asio::error::eof,
      [&](boost::system::error_code ec)
      {
        ec2 = ec;
      });

  boost::system::error_code ec3;
  bool b5 = ch1.try_receive(
      [&](boost::system::error_code ec)
      {
        ec3 = ec;
      });

  BOOST_ASIO_CHECK(b5);
  BOOST_ASIO_CHECK(ec3 == boost::asio::error::eof);

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(!ec2);
}

void unbuffered_non_immediate_receive()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec2 = ec;
        s2 = std::move(s);
      });

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");
}

void unbuffered_immediate_receive()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      bind_immediate_executor(system_executor(),
        [&](boost::system::error_code ec, std::string s)
        {
          ec2 = ec;
          s2 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);
}

void unbuffered_executor_receive()
{
  io_context ctx;
  io_context ctx2;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      bind_executor(ctx2,
        [&](boost::system::error_code ec, std::string s)
        {
          ec2 = ec;
          s2 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx2.run();

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");
}

void unbuffered_non_immediate_send()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec1 = ec;
        s1 = std::move(s);
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s2),
      [&](boost::system::error_code ec)
      {
        ec2 = ec;
      });

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "0123456789");
  BOOST_ASIO_CHECK(!ec2);
}

void unbuffered_immediate_send()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec1 = ec;
        s1 = std::move(s);
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s2),
      bind_immediate_executor(system_executor(),
        [&](boost::system::error_code ec)
        {
          ec2 = ec;
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(!ec2);

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "0123456789");
}

void unbuffered_executor_send()
{
  io_context ctx;
  io_context ctx2;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec1 = ec;
        s1 = std::move(s);
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s2),
      bind_executor(ctx2,
        [&](boost::system::error_code ec)
        {
          ec2 = ec;
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "0123456789");
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx2.run();

  BOOST_ASIO_CHECK(!ec2);
}

void buffered_non_immediate_receive()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      [&](boost::system::error_code ec, std::string s)
      {
        ec2 = ec;
        s2 = std::move(s);
      });

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");
}

void buffered_immediate_receive()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      bind_immediate_executor(system_executor(),
        [&](boost::system::error_code ec, std::string s)
        {
          ec2 = ec;
          s2 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");

  ctx.restart();
  ctx.run();
}

void buffered_executor_receive()
{
  io_context ctx;
  io_context ctx2;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      bind_executor(ctx2,
        [&](boost::system::error_code ec, std::string s)
        {
          ec2 = ec;
          s2 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx2.run();

  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");
}

void buffered_non_immediate_send()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      [&](boost::system::error_code ec)
      {
        ec1 = ec;
      });

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(!ec1);
}

void buffered_immediate_send()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      bind_immediate_executor(system_executor(),
        [&](boost::system::error_code ec)
        {
          ec1 = ec;
        }));

  BOOST_ASIO_CHECK(!ec1);

  ctx.run();
}

void buffered_executor_send()
{
  io_context ctx;
  io_context ctx2;

  channel<void(boost::system::error_code, std::string)> ch1(ctx, 1);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1 = "0123456789";
  ch1.async_send(boost::asio::error::eof, std::move(s1),
      bind_executor(ctx2,
        [&](boost::system::error_code ec)
        {
          ec1 = ec;
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx2.run();

  BOOST_ASIO_CHECK(!ec1);
}

void try_send_via_dispatch()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      bind_executor(boost::asio::system_executor(),
        [&](boost::system::error_code ec, std::string s)
        {
          ec1 = ec;
          s1 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.poll();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  std::string s2 = "0123456789";
  ch1.try_send_via_dispatch(boost::asio::error::eof, std::move(s2));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "0123456789");
  BOOST_ASIO_CHECK(s2.empty());
}

void try_send_n_via_dispatch()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      bind_executor(boost::asio::system_executor(),
        [&](boost::system::error_code ec, std::string s)
        {
          ec1 = ec;
          s1 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2;
  ch1.async_receive(
      bind_executor(boost::asio::system_executor(),
        [&](boost::system::error_code ec, std::string s)
        {
          ec2 = ec;
          s2 = std::move(s);
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  ctx.poll();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  std::string s3 = "0123456789";
  ch1.try_send_n_via_dispatch(2, boost::asio::error::eof, std::move(s3));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "0123456789");
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s2 == "0123456789");
  BOOST_ASIO_CHECK(s3.empty());
}

struct multi_signature_handler
{
  std::string* s_;
  boost::system::error_code* ec_;

  void operator()(std::string s)
  {
    *s_ = s;
  }

  void operator()(boost::system::error_code ec)
  {
    *ec_ = ec;
  }
};

void implicit_error_signature_channel_test()
{
  io_context ctx;

  channel<void(std::string)> ch1(ctx);

  BOOST_ASIO_CHECK(ch1.is_open());
  BOOST_ASIO_CHECK(!ch1.ready());

  bool b1 = ch1.try_send("hello");

  BOOST_ASIO_CHECK(!b1);

  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  bool b2 = ch1.try_send(std::move(s1));

  BOOST_ASIO_CHECK(!b2);
  BOOST_ASIO_CHECK(!s1.empty());

  std::string s2;
  boost::system::error_code ec1 = boost::asio::error::would_block;
  multi_signature_handler h1 = {&s2, &ec1};
  ch1.async_receive(h1);

  bool b3 = ch1.try_send(std::move(s1));

  BOOST_ASIO_CHECK(b3);
  BOOST_ASIO_CHECK(s1.empty());

  ctx.run();

  BOOST_ASIO_CHECK(s2 == "abcdefghijklmnopqrstuvwxyz");
  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);

  std::string s3;
  boost::system::error_code ec2;
  multi_signature_handler h2 = {&s3, &ec2};
  bool b4 = ch1.try_receive(h2);

  BOOST_ASIO_CHECK(!b4);

  std::string s4 = "zyxwvutsrqponmlkjihgfedcba";
  boost::system::error_code ec3;
  ch1.async_send(std::move(s4),
      [&](boost::system::error_code ec)
      {
        ec3 = ec;
      });

  std::string s5;
  boost::system::error_code ec4 = boost::asio::error::would_block;
  multi_signature_handler h3 = {&s5, &ec4};
  bool b5 = ch1.try_receive(h3);

  BOOST_ASIO_CHECK(b5);
  BOOST_ASIO_CHECK(ec4 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(s5 == "zyxwvutsrqponmlkjihgfedcba");

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(!ec3);

  std::string s6;
  boost::system::error_code ec5 = boost::asio::error::would_block;
  multi_signature_handler h4 = {&s6, &ec5};
  ch1.async_receive(h4);

  ch1.close();

  ctx.restart();
  ctx.run();

  BOOST_ASIO_CHECK(s6.empty());
  BOOST_ASIO_CHECK(ec5 == boost::asio::experimental::channel_errc::channel_closed);
}

void channel_with_any_completion_handler_test()
{
  io_context ctx;

  channel<void(boost::system::error_code, std::string)> ch1(ctx);

  boost::system::error_code ec1 = boost::asio::error::would_block;
  std::string s1;
  ch1.async_receive(
      boost::asio::any_completion_handler<
        void(boost::system::error_code, std::string)>(
          [&](boost::system::error_code ec, std::string s)
          {
            ec1 = ec;
            s1 = std::move(s);
          }));

  boost::system::error_code ec2 = boost::asio::error::would_block;
  std::string s2 = "zyxwvutsrqponmlkjihgfedcba";
  ch1.async_send(boost::asio::error::eof, std::move(s2),
      boost::asio::any_completion_handler<void(boost::system::error_code)>(
        [&](boost::system::error_code ec)
        {
          ec2 = ec;
        }));

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::would_block);
  BOOST_ASIO_CHECK(ec2 == boost::asio::error::would_block);

  ctx.run();

  BOOST_ASIO_CHECK(ec1 == boost::asio::error::eof);
  BOOST_ASIO_CHECK(s1 == "zyxwvutsrqponmlkjihgfedcba");
  BOOST_ASIO_CHECK(!ec2);
}

BOOST_ASIO_TEST_SUITE
(
  "experimental/channel",
  BOOST_ASIO_TEST_CASE(unbuffered_channel_test)
  BOOST_ASIO_TEST_CASE(buffered_channel_test)
  BOOST_ASIO_TEST_CASE(buffered_error_channel_test)
  BOOST_ASIO_TEST_CASE(unbuffered_non_immediate_receive)
  BOOST_ASIO_TEST_CASE(unbuffered_immediate_receive)
  BOOST_ASIO_TEST_CASE(unbuffered_executor_receive)
  BOOST_ASIO_TEST_CASE(unbuffered_non_immediate_send)
  BOOST_ASIO_TEST_CASE(unbuffered_immediate_send)
  BOOST_ASIO_TEST_CASE(unbuffered_executor_send)
  BOOST_ASIO_TEST_CASE(buffered_non_immediate_receive)
  BOOST_ASIO_TEST_CASE(buffered_immediate_receive)
  BOOST_ASIO_TEST_CASE(buffered_executor_receive)
  BOOST_ASIO_TEST_CASE(buffered_non_immediate_send)
  BOOST_ASIO_TEST_CASE(buffered_immediate_send)
  BOOST_ASIO_TEST_CASE(buffered_executor_send)
  BOOST_ASIO_TEST_CASE(try_send_via_dispatch)
  BOOST_ASIO_TEST_CASE(try_send_n_via_dispatch)
  BOOST_ASIO_TEST_CASE(implicit_error_signature_channel_test)
  BOOST_ASIO_TEST_CASE(channel_with_any_completion_handler_test)
)
