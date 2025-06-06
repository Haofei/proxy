//
// blocking_adaptation.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~
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
#include <boost/asio/execution/blocking_adaptation.hpp>

#include <boost/asio/prefer.hpp>
#include <boost/asio/query.hpp>
#include <boost/asio/require.hpp>
#include "../unit_test.hpp"

namespace exec = boost::asio::execution;

typedef exec::blocking_adaptation_t s;
typedef exec::blocking_adaptation_t::disallowed_t n1;
typedef exec::blocking_adaptation_t::allowed_t n2;

struct ex_nq_nr
{
  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_nq_nr&, const ex_nq_nr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_nq_nr&, const ex_nq_nr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <>
struct equality_comparable<ex_nq_nr>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename Function>
struct execute_member<ex_nq_nr, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename ResultType, typename ParamType, typename Result>
struct ex_cq_nr
{
  static constexpr ResultType query(ParamType) noexcept
  {
    return Result();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_cq_nr&, const ex_cq_nr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_cq_nr&, const ex_cq_nr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <typename ResultType, typename ParamType, typename Result>
struct equality_comparable<ex_cq_nr<ResultType, ParamType, Result> >
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename ResultType, typename ParamType,
    typename Result, typename Function>
struct execute_member<ex_cq_nr<ResultType, ParamType, Result>, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_STATIC_CONSTEXPR_MEMBER_TRAIT)

template <typename ResultType, typename ParamType,
  typename Result, typename Param>
struct query_static_constexpr_member<
  ex_cq_nr<ResultType, ParamType, Result>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, ParamType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef Result result_type; // Must return raw result type.

  static constexpr result_type value()
  {
    return Result();
  }
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_STATIC_CONSTEXPR_MEMBER_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename ResultType, typename ParamType, typename Result>
struct ex_mq_nr
{
  ResultType query(ParamType) const noexcept
  {
    return Result();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_mq_nr&, const ex_mq_nr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_mq_nr&, const ex_mq_nr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <typename ResultType, typename ParamType, typename Result>
struct equality_comparable<ex_mq_nr<ResultType, ParamType, Result> >
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename ResultType, typename ParamType,
    typename Result, typename Function>
struct execute_member<ex_mq_nr<ResultType, ParamType, Result>, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_MEMBER_TRAIT)

template <typename ResultType, typename ParamType,
  typename Result, typename Param>
struct query_member<
  ex_mq_nr<ResultType, ParamType, Result>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, ParamType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ResultType result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_MEMBER_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename ResultType, typename ParamType, typename Result>
struct ex_fq_nr
{
  friend ResultType query(const ex_fq_nr&, ParamType) noexcept
  {
    return Result();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_fq_nr&, const ex_fq_nr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_fq_nr&, const ex_fq_nr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <typename ResultType, typename ParamType, typename Result>
struct equality_comparable<ex_fq_nr<ResultType, ParamType, Result> >
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename ResultType, typename ParamType,
    typename Result, typename Function>
struct execute_member<ex_fq_nr<ResultType, ParamType, Result>, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

template <typename ResultType, typename ParamType,
  typename Result, typename Param>
struct query_free<
  ex_fq_nr<ResultType, ParamType, Result>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, ParamType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ResultType result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename CurrentType, typename OtherType>
struct ex_mq_mr
{
  CurrentType query(CurrentType) const noexcept
  {
    return CurrentType();
  }

  CurrentType query(OtherType) const noexcept
  {
    return CurrentType();
  }

  ex_mq_mr<CurrentType, OtherType> require(
      CurrentType) const noexcept
  {
    return ex_mq_mr<CurrentType, OtherType>();
  }

  ex_mq_mr<OtherType, CurrentType> require(
      OtherType) const noexcept
  {
    return ex_mq_mr<OtherType, CurrentType>();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_mq_mr&, const ex_mq_mr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_mq_mr&, const ex_mq_mr&) noexcept
  {
    return false;
  }
};

template <typename CurrentType>
struct ex_mq_mr<CurrentType, CurrentType>
{
  CurrentType query(CurrentType) const noexcept
  {
    return CurrentType();
  }

  ex_mq_mr<CurrentType, CurrentType> require(
      CurrentType) const noexcept
  {
    return ex_mq_mr<CurrentType, CurrentType>();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_mq_mr&, const ex_mq_mr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_mq_mr&, const ex_mq_mr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <typename CurrentType, typename OtherType>
struct equality_comparable<ex_mq_mr<CurrentType, OtherType> >
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename CurrentType, typename OtherType, typename Function>
struct execute_member<ex_mq_mr<CurrentType, OtherType>, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_MEMBER_TRAIT)

template <typename CurrentType, typename OtherType, typename Param>
struct query_member<
  ex_mq_mr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, CurrentType>::value
      || boost::asio::is_convertible<Param, OtherType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef CurrentType result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_REQUIRE_MEMBER_TRAIT)

template <typename CurrentType, typename OtherType, typename Param>
struct require_member<
  ex_mq_mr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, CurrentType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_mq_mr<CurrentType, OtherType> result_type;
};

template <typename CurrentType, typename OtherType, typename Param>
struct require_member<
  ex_mq_mr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, OtherType>::value
      && !boost::asio::is_same<CurrentType, OtherType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_mq_mr<OtherType, CurrentType> result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_REQUIRE_MEMBER_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename CurrentType, typename OtherType>
struct ex_fq_fr
{
  friend CurrentType query(const ex_fq_fr&, CurrentType) noexcept
  {
    return CurrentType();
  }

  friend CurrentType query(const ex_fq_fr&, OtherType) noexcept
  {
    return CurrentType();
  }

  friend ex_fq_fr<CurrentType, OtherType> require(
      const ex_fq_fr&, CurrentType) noexcept
  {
    return ex_fq_fr<CurrentType, OtherType>();
  }

  friend ex_fq_fr<OtherType, CurrentType> require(
      const ex_fq_fr&, OtherType) noexcept
  {
    return ex_fq_fr<OtherType, CurrentType>();
  }

  friend ex_fq_fr<CurrentType, OtherType> prefer(
      const ex_fq_fr&, CurrentType) noexcept
  {
    return ex_fq_fr<CurrentType, OtherType>();
  }

  friend ex_fq_fr<OtherType, CurrentType> prefer(
      const ex_fq_fr&, OtherType) noexcept
  {
    return ex_fq_fr<OtherType, CurrentType>();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_fq_fr&, const ex_fq_fr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_fq_fr&, const ex_fq_fr&) noexcept
  {
    return false;
  }
};

template <typename CurrentType>
struct ex_fq_fr<CurrentType, CurrentType>
{
  friend CurrentType query(const ex_fq_fr&, CurrentType) noexcept
  {
    return CurrentType();
  }

  friend ex_fq_fr<CurrentType, CurrentType> require(
      const ex_fq_fr&, CurrentType) noexcept
  {
    return ex_fq_fr<CurrentType, CurrentType>();
  }

  friend ex_fq_fr<CurrentType, CurrentType> prefer(
      const ex_fq_fr&, CurrentType) noexcept
  {
    return ex_fq_fr<CurrentType, CurrentType>();
  }

  template <typename F>
  void execute(const F&) const
  {
  }

  friend bool operator==(const ex_fq_fr&, const ex_fq_fr&) noexcept
  {
    return true;
  }

  friend bool operator!=(const ex_fq_fr&, const ex_fq_fr&) noexcept
  {
    return false;
  }
};

namespace boost {
namespace asio {
namespace traits {

#if !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

template <typename CurrentType, typename OtherType>
struct equality_comparable<ex_fq_fr<CurrentType, OtherType> >
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

template <typename CurrentType, typename OtherType, typename Function>
struct execute_member<ex_fq_fr<CurrentType, OtherType>, Function>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = false;

  typedef void result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

template <typename CurrentType, typename OtherType, typename Param>
struct query_free<
  ex_fq_fr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, CurrentType>::value
      || boost::asio::is_convertible<Param, OtherType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef CurrentType result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_QUERY_FREE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_REQUIRE_FREE_TRAIT)

template <typename CurrentType, typename OtherType, typename Param>
struct require_free<
  ex_fq_fr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, CurrentType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_fq_fr<CurrentType, OtherType> result_type;
};

template <typename CurrentType, typename OtherType, typename Param>
struct require_free<
  ex_fq_fr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, OtherType>::value
      && !boost::asio::is_same<CurrentType, OtherType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_fq_fr<OtherType, CurrentType> result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_REQUIRE_FREE_TRAIT)

#if !defined(BOOST_ASIO_HAS_DEDUCED_PREFER_FREE_TRAIT)

template <typename CurrentType, typename OtherType, typename Param>
struct prefer_free<
  ex_fq_fr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, CurrentType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_fq_fr<CurrentType, OtherType> result_type;
};

template <typename CurrentType, typename OtherType, typename Param>
struct prefer_free<
  ex_fq_fr<CurrentType, OtherType>, Param,
  typename boost::asio::enable_if<
    boost::asio::is_convertible<Param, OtherType>::value
      && !boost::asio::is_same<CurrentType, OtherType>::value
  >::type>
{
  static constexpr bool is_valid = true;
  static constexpr bool is_noexcept = true;

  typedef ex_fq_fr<OtherType, CurrentType> result_type;
};

#endif // !defined(BOOST_ASIO_HAS_DEDUCED_PREFER_FREE_TRAIT)

} // namespace traits
} // namespace asio
} // namespace boost

template <typename Executor, typename Param, bool ExpectedResult>
void test_can_query()
{
  constexpr bool b1 =
    boost::asio::can_query<Executor, Param>::value;
  BOOST_ASIO_CHECK(b1 == ExpectedResult);

  constexpr bool b2 =
    boost::asio::can_query<const Executor, Param>::value;
  BOOST_ASIO_CHECK(b2 == ExpectedResult);

  constexpr bool b3 =
    boost::asio::can_query<Executor&, Param>::value;
  BOOST_ASIO_CHECK(b3 == ExpectedResult);

  constexpr bool b4 =
    boost::asio::can_query<const Executor&, Param>::value;
  BOOST_ASIO_CHECK(b4 == ExpectedResult);
}

template <typename Executor, typename Param, typename ExpectedResult>
void test_query()
{
  exec::blocking_adaptation_t result1 = boost::asio::query(Executor(), Param());
  BOOST_ASIO_CHECK(result1 == ExpectedResult());

  Executor ex1 = {};
  exec::blocking_adaptation_t result2 = boost::asio::query(ex1, Param());
  BOOST_ASIO_CHECK(result2 == ExpectedResult());

  const Executor ex2 = {};
  exec::blocking_adaptation_t result3 = boost::asio::query(ex2, Param());
  BOOST_ASIO_CHECK(result3 == ExpectedResult());
}

template <typename Executor, typename Param, typename ExpectedResult>
void test_constexpr_query()
{
  constexpr Executor ex1 = {};
  constexpr exec::blocking_adaptation_t result1 = boost::asio::query(ex1, Param());
  BOOST_ASIO_CHECK(result1 == ExpectedResult());
}

template <typename Executor, typename Param, bool ExpectedResult>
void test_can_require()
{
  constexpr bool b1 =
    boost::asio::can_require<Executor, Param>::value;
  BOOST_ASIO_CHECK(b1 == ExpectedResult);

  constexpr bool b2 =
    boost::asio::can_require<const Executor, Param>::value;
  BOOST_ASIO_CHECK(b2 == ExpectedResult);

  constexpr bool b3 =
    boost::asio::can_require<Executor&, Param>::value;
  BOOST_ASIO_CHECK(b3 == ExpectedResult);

  constexpr bool b4 =
    boost::asio::can_require<const Executor&, Param>::value;
  BOOST_ASIO_CHECK(b4 == ExpectedResult);
}

void do_nothing()
{
}

template <typename Executor, typename Param, typename ExpectedResult>
void test_require()
{
  BOOST_ASIO_CHECK(
      boost::asio::query(
        boost::asio::require(Executor(), Param()),
        Param()) == ExpectedResult());

  Executor ex1 = {};

  BOOST_ASIO_CHECK(
      boost::asio::query(
        boost::asio::require(ex1, Param()),
        Param()) == ExpectedResult());

  BOOST_ASIO_CHECK((
      boost::asio::execution::is_executor<
        typename boost::asio::decay<
          typename boost::asio::require_result<Executor&, Param>::type
        >::type
      >::value));

  const Executor ex2 = {};

  BOOST_ASIO_CHECK(
      boost::asio::query(
        boost::asio::require(ex2, Param()),
        Param()) == ExpectedResult());

  BOOST_ASIO_CHECK((
      boost::asio::execution::is_executor<
        typename boost::asio::decay<
          typename boost::asio::require_result<const Executor&, Param>::type
        >::type
      >::value));
}

template <typename Executor, typename Param, bool ExpectedResult>
void test_can_prefer()
{
  constexpr bool b1 =
    boost::asio::can_prefer<Executor, Param>::value;
  BOOST_ASIO_CHECK(b1 == ExpectedResult);

  constexpr bool b2 =
    boost::asio::can_prefer<const Executor, Param>::value;
  BOOST_ASIO_CHECK(b2 == ExpectedResult);

  constexpr bool b3 =
    boost::asio::can_prefer<Executor&, Param>::value;
  BOOST_ASIO_CHECK(b3 == ExpectedResult);

  constexpr bool b4 =
    boost::asio::can_prefer<const Executor&, Param>::value;
  BOOST_ASIO_CHECK(b4 == ExpectedResult);
}

template <typename Executor, typename Param, typename ExpectedResult>
void test_prefer()
{
  BOOST_ASIO_CHECK(
      s(boost::asio::query(
        boost::asio::prefer(Executor(), Param()),
          s())) == s(ExpectedResult()));

  Executor ex1 = {};
  BOOST_ASIO_CHECK(
      s(boost::asio::query(
        boost::asio::prefer(ex1, Param()),
          s())) == s(ExpectedResult()));

  const Executor ex2 = {};
  BOOST_ASIO_CHECK(
      s(boost::asio::query(
        boost::asio::prefer(ex2, Param()),
          s())) == s(ExpectedResult()));
}

void test_vars()
{
  BOOST_ASIO_CHECK(s() == exec::blocking_adaptation);
  BOOST_ASIO_CHECK(s() != exec::blocking_adaptation.disallowed);
  BOOST_ASIO_CHECK(s() != exec::blocking_adaptation.allowed);
  BOOST_ASIO_CHECK(n1() == exec::blocking_adaptation.disallowed);
  BOOST_ASIO_CHECK(n1() != exec::blocking_adaptation.allowed);
  BOOST_ASIO_CHECK(n2() == exec::blocking_adaptation.allowed);
  BOOST_ASIO_CHECK(n2() != exec::blocking_adaptation.disallowed);
}

BOOST_ASIO_TEST_SUITE
(
  "blocking_adaptation",

  BOOST_ASIO_TEST_CASE3(test_can_query<ex_nq_nr, s, true>)
  BOOST_ASIO_TEST_CASE3(test_can_query<ex_nq_nr, n1, true>)
  BOOST_ASIO_TEST_CASE3(test_can_query<ex_nq_nr, n2, false>)

  BOOST_ASIO_TEST_CASE3(test_query<ex_nq_nr, s, n1>)
  BOOST_ASIO_TEST_CASE3(test_query<ex_nq_nr, n1, n1>)

  BOOST_ASIO_TEST_CASE3(test_constexpr_query<ex_nq_nr, s, n1>)
  BOOST_ASIO_TEST_CASE3(test_constexpr_query<ex_nq_nr, n1, n1>)

  BOOST_ASIO_TEST_CASE3(test_can_require<ex_nq_nr, s, false>)
  BOOST_ASIO_TEST_CASE3(test_can_require<ex_nq_nr, n1, true>)
  BOOST_ASIO_TEST_CASE3(test_can_require<ex_nq_nr, n2, true>)

  BOOST_ASIO_TEST_CASE3(test_require<ex_nq_nr, n1, n1>)
  BOOST_ASIO_TEST_CASE3(test_require<ex_nq_nr, n2, n2>)

  BOOST_ASIO_TEST_CASE3(test_can_prefer<ex_nq_nr, s, false>)
  BOOST_ASIO_TEST_CASE3(test_can_prefer<ex_nq_nr, n1, true>)
  BOOST_ASIO_TEST_CASE3(test_can_prefer<ex_nq_nr, n2, false>)

  BOOST_ASIO_TEST_CASE3(test_prefer<ex_nq_nr, n1, n1>)

  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n1, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n2, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_cq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n1, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n2, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n2, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n1, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n1, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n2, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n2, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_cq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n1, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n2, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n2, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n1, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n1, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n2, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n2, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_constexpr_query<ex_cq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n2, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_cq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, n1, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, n1, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<n1, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_cq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, s, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<s, n2, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n1, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_cq_nr<n2, s, n2>, n2, false>)

  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<s, n2, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_cq_nr<n2, s, n2>, n1, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n1, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n2, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_mq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n1, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n2, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n2, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n1, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n1, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n2, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n2, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_mq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n1, s, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n2, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_mq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, n1, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, n1, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<n1, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_mq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, s, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<s, n2, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n1, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_mq_nr<n2, s, n2>, n2, false>)

  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<s, n2, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_mq_nr<n2, s, n2>, n1, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n1, s, n1>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n2, s, n2>, s, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_query<ex_fq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n1, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n2, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n2, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n1, s, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n1, s, n1>, n2, n1>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n2, s, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n2, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_query<ex_fq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, s, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<s, n2, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n1, s, n1>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n1, s, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n2, s, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE5(test_can_require<ex_fq_nr<n2, s, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, s, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, n1, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, n1, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<s, n2, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<n1, s, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE5(test_require<ex_fq_nr<n2, s, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, s, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<s, n2, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n1, s, n1>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n1, s, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n1, s, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n2, s, n2>, s, false>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n2, s, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE5(test_can_prefer<ex_fq_nr<n2, s, n2>, n2, false>)

  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, s, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, n1, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<s, n2, n2>, n1, n2>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<n1, s, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE5(test_prefer<ex_fq_nr<n2, s, n2>, n1, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n1>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n2>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n1>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n2>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_mq_mr<n2, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n1, n2>, s, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n2, n1>, s, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_mq_mr<n2, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_mq_mr<n2, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n1, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n1, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_mq_mr<n2, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_mq_mr<n2, n2>, n2, false>)

  BOOST_ASIO_TEST_CASE4(test_prefer<ex_mq_mr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_mq_mr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_mq_mr<n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_mq_mr<n2, n2>, n1, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n1>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n2>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n1>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n2>, s, true>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE4(test_can_query<ex_fq_fr<n2, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n1, n1>, s, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n1, n2>, s, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n2, n1>, s, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n2, n2>, s, n2>)
  BOOST_ASIO_TEST_CASE4(test_query<ex_fq_fr<n2, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n1, n2>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n1>, n2, true>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n2>, n1, false>)
  BOOST_ASIO_TEST_CASE4(test_can_require<ex_fq_fr<n2, n2>, n2, true>)

  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n1, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n1, n2>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n2, n1>, n2, n2>)
  BOOST_ASIO_TEST_CASE4(test_require<ex_fq_fr<n2, n2>, n2, n2>)

  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n1, n2>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n1>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n1>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n1>, n2, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n2>, s, false>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n2>, n1, true>)
  BOOST_ASIO_TEST_CASE4(test_can_prefer<ex_fq_fr<n2, n2>, n2, false>)

  BOOST_ASIO_TEST_CASE4(test_prefer<ex_fq_fr<n1, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_fq_fr<n1, n2>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_fq_fr<n2, n1>, n1, n1>)
  BOOST_ASIO_TEST_CASE4(test_prefer<ex_fq_fr<n2, n2>, n1, n2>)

  BOOST_ASIO_TEST_CASE(test_vars)
)
