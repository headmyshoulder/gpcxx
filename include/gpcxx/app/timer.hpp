/*
  gpcxx/app/timer.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_APP_TIMER_HPP_DEFINED
#define GPCXX_APP_TIMER_HPP_DEFINED


#include <chrono>

namespace gpcxx {

class timer
{
    typedef std::chrono::high_resolution_clock clock_type;
    clock_type::time_point m_start_time;

    template< class T >
    static inline double get_seconds( T t )
    {
        return double( std::chrono::duration_cast< std::chrono::milliseconds >( t ).count() ) / 1000.0;
    }

public:

    timer( void ) : m_start_time( clock_type::now() ) { }

    inline double seconds( void ) const
    {
        return get_seconds( clock_type::now() - m_start_time );
    }

    inline void restart( void )
    {
        m_start_time = clock_type::now();
    }
};




} // namespace gpcxx



#endif // GPCXX_APP_TIMER_HPP_DEFINED
