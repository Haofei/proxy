//
// Copyright (c) 2019-2025 Ruben Perez Hidalgo (rubenperez038 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_MYSQL_IMPL_INTERNAL_SANSIO_READ_SOME_ROWS_DYNAMIC_HPP
#define BOOST_MYSQL_IMPL_INTERNAL_SANSIO_READ_SOME_ROWS_DYNAMIC_HPP

#include <boost/mysql/error_code.hpp>
#include <boost/mysql/rows_view.hpp>

#include <boost/mysql/detail/access.hpp>
#include <boost/mysql/detail/algo_params.hpp>
#include <boost/mysql/detail/execution_processor/execution_processor.hpp>
#include <boost/mysql/detail/execution_processor/execution_state_impl.hpp>

#include <boost/mysql/impl/internal/sansio/connection_state_data.hpp>
#include <boost/mysql/impl/internal/sansio/read_some_rows.hpp>

#include <cstddef>

namespace boost {
namespace mysql {
namespace detail {

class read_some_rows_dynamic_algo : public read_some_rows_algo
{
public:
    // This algorithm is always top-level
    read_some_rows_dynamic_algo(read_some_rows_dynamic_algo_params params) noexcept
        : read_some_rows_algo(read_some_rows_algo_params{params.exec_st, output_ref()})
    {
    }

    rows_view result(const connection_state_data& st) const
    {
        std::size_t num_rows = read_some_rows_algo::result(st);
        std::size_t num_cols = static_cast<const execution_state_impl&>(processor()).meta().size();
        return access::construct<rows_view>(st.shared_fields.data(), num_rows * num_cols, num_cols);
    }
};

}  // namespace detail
}  // namespace mysql
}  // namespace boost

#endif
