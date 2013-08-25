/*
  gpcxx/util/array_unpack.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_UTIL_ARRAY_UNPACK_HPP_DEFINED
#define GPCXX_UTIL_ARRAY_UNPACK_HPP_DEFINED

#include <array>


namespace gpcxx {
    
namespace array_unpacker_details {
    
    
    template< size_t I , size_t N , typename Value , typename Func , typename ... Args >
    struct unpacker;

    template< size_t I , size_t N , typename Value , typename Func , typename ... Args >
    inline Value call_unpacker( std::array< Value , N > const& t , Func f , Args&& ... args )
    {
        unpacker< I , N , Value , Func , Args ... > u;
        return u( t , f , args ... );
    }

    template< size_t I , size_t N , typename Value , typename Func , typename ... Args >
    struct unpacker
    {
        inline Value operator()( std::array< Value , N > const& t , Func f , Args&& ... args ) const
        {
            return call_unpacker< I - 1 >( t , f , t[I-1] , args ... );
        }
    };

    template< size_t N , typename Value , typename Func , typename ... Args >
    struct unpacker< 0 , N , Value , Func , Args ... >
    {
        inline Value operator()( std::array< Value , N > const &t , Func f , Args&& ... args ) const
        {
            return f( args ... );
        }
    };

} // namespace array_unpacker_details


template< size_t N , typename Value , typename Func >
inline Value array_unpack( std::array< Value , N > const& t , Func f )
{
    return array_unpacker_details::call_unpacker< N - 1 >( t , f , t[N-1] );
}




} // namespace gpcxx


#endif // GPCXX_UTIL_ARRAY_UNPACK_HPP_DEFINED
