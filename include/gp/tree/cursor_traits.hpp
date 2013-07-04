/*
  gp/tree/cursor_traits.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_CURSOR_TRAITS_HPP_DEFINED
#define GP_TREE_CURSOR_TRAITS_HPP_DEFINED


namespace gp {

template< typename Cursor >
struct is_cursor : public std::false_type
{
};


template< typename Cursor >
struct cursor_value
{
    typedef typename Cursor::value_type type;
};


} // namespace gp


#endif // GP_TREE_CURSOR_TRAITS_HPP_DEFINED
