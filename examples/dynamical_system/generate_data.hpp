/*
 * gpcxx/examples/dynamical_system/generate_data.hpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_GENERATE_DATA_HPP_INCLUDED
#define GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_GENERATE_DATA_HPP_INCLUDED

#include <array>
#include <vector>
#include <utility>


namespace dynsys {

    static const size_t dim = 3;
    using state_type = std::array< double , dim >;
    using state_container = std::vector< state_type >;
    using data_type = std::pair< state_container , state_container >;
    using norm_type = std::array< std::pair< double , double > , dim >;
    
    
    inline void lorenz( const state_type &x , state_type &dxdt , double t )
    {
        const double sigma = 10.0;
        const double R = 28.0;
        const double b = 8.0 / 3.0;
        dxdt[0] = sigma * ( x[1] - x[0] );
        dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
        dxdt[2] = -b * x[2] + x[0] * x[1];
    }
    
    
    data_type generate_data( void );
    void plot_data( data_type const& data );
    norm_type normalize_data( state_container& data );


} // namespace dynsys 


#endif // GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_GENERATE_DATA_HPP_INCLUDED
