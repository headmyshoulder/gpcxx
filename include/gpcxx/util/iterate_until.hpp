/*
  gpcxx/util/iterate_until.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_UTIL_ITERATE_UNTIL_HPP_DEFINED
#define GPCXX_UTIL_ITERATE_UNTIL_HPP_DEFINED

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/equal_to.hpp>

namespace gpcxx {

namespace detail {
    
template< typename First , typename Last , typename F , typename Enable = void >
struct iterate_until_impl;

template< typename Iter , typename F >
struct iterate_until_impl< Iter , Iter , F >
{
    inline bool operator()( Iter first , Iter Last , F f ) const
    {
        return false;
    }
};

template< typename First , typename Last , typename F >
struct iterate_until_impl< First , Last , F >
{
    inline bool operator()( First first , Last last , F f ) const
    {
        if( f( boost::fusion::deref( first ) ) ) return true;
        else
            return iterate_until_impl< 
                typename boost::fusion::result_of::next< First >::type ,
                Last , F >()( boost::fusion::next( first ) , last , f );
    }
};

template< typename First , typename Last , typename F >
inline bool iterate_until( First first , Last last , F f )
{
    return iterate_until_impl< First , Last , F >()( first , last , f );
}

} // namespace detail

template< typename Seq , typename F >
inline bool iterate_until( Seq const& s , F f )
{
    static_assert( boost::fusion::traits::is_sequence< Seq >::value , "Sequence must be a Boost.Fusion sequence" );
    return gpcxx::detail::iterate_until( boost::fusion::begin( s ) , boost::fusion::end( s ) , f );
}


} // namespace gpcxx


#endif // GPCXX_UTIL_ITERATE_UNTIL_HPP_DEFINED
