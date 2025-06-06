/* Copyright 2016-2024 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/poly_collection for library home page.
 */

#ifndef BOOST_POLY_COLLECTION_DETAIL_IS_ACCEPTABLE_HPP
#define BOOST_POLY_COLLECTION_DETAIL_IS_ACCEPTABLE_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/mp11/algorithm.hpp>
#include <boost/poly_collection/detail/is_closed_collection.hpp>
#include <boost/poly_collection/detail/is_moveable.hpp>
#include <type_traits>

namespace boost{

namespace poly_collection{

namespace detail{

/* is_acceptable can be further specialized by (open collection) Model when
 * the std type_traits classes fail to give the right info (as it can happen
 * with class templates whose nominally existing operators do not compile for
 * certain instantiations).
 */

template<typename T,typename Model,typename=void>
struct is_acceptable:std::integral_constant<
  bool,
  Model::template is_implementation<T>::value&&is_moveable<T>::value
>{};

/* Closed collections are defined by having a compile-time fixed list of
 * acceptable types. 
 */

template<typename T,typename Model>
struct is_acceptable<
  T,Model,
  typename std::enable_if<is_closed_collection<Model>::value>::type
>:mp11::mp_contains<typename Model::acceptable_type_list,T>{};

} /* namespace poly_collection::detail */

} /* namespace poly_collection */

} /* namespace boost */

#endif
