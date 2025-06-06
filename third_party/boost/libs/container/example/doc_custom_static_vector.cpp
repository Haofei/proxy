//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2013-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//[doc_custom_static_vector
#include <boost/container/static_vector.hpp>

//Make sure assertions are active
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

int main ()
{
   using namespace boost::container;

   //This option specifies the desired alignment for value_type
   typedef static_vector_options< inplace_alignment<16u> >::type alignment_16_option_t;

   //Check 16 byte alignment option
   static_vector<int, 10, alignment_16_option_t > sv;
   assert(((std::size_t)sv.data() % 16u) == 0);

   //This static_vector won't throw on overflow, for maximum performance
   typedef static_vector_options< throw_on_overflow<false> >::type no_throw_options_t;

   //Create static_vector with no throw on overflow
   static_vector<int, 10, no_throw_options_t > sv2;

   //This options specifies that internal `size()` can be represented by an unsigned char
   //instead of the default `std::size_t`.
   typedef static_vector_options< stored_size<unsigned char> >::type stored_size_uchar_t;

   //Create static_vector that internally stores `size()` in an unsigned char
   //because `capacity()` will be less thatn 256.
   static_vector<unsigned char, 10, stored_size_uchar_t> sv3;

   return 0;
}
//]
