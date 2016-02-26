/*
 * generate_data.cpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "generate_data.hpp"

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>

#include <iostream>
#include <cmath>
#include <cassert>

namespace dynsys {
    
using stepper_type = boost::numeric::odeint::runge_kutta4< state_type > ;
    
    
data_type generate_data( void )
{
    data_type data;
    state_type x {{ 10.0 , 10.0 , 10.0 }};
    state_type dxdt;
    double dt = 0.1;
    stepper_type stepper;
    for( double t = 0.0 ; t<100.0 ; t+= dt )
    {
        lorenz( x , dxdt , t );
        data.first.push_back( x );
        data.second.push_back( dxdt );
        stepper.do_step( lorenz , x , dxdt , t , dt );
    }
    return data;
}

void plot_data( data_type const& data , std::ostream& out )
{
    for( size_t i=0 ; i<data.first.size() ; ++i )
    {
        state_type const& x = data.first[i];
        state_type const& dxdt = data.second[i];
        out << x[0] << " " << x[1] << " " << x[2] << " " << dxdt[0] << " " << dxdt[1] << " " << dxdt[2] << "\n";
    }
}

std::array< std::pair< double , double > , dim > normalize_data( state_container& data )
{
    std::array< std::pair< double , double > , dim > means;
    for( size_t j=0 ; j<dim ; ++j )
    {
        means[j].first = means[j].second = 0.0;
    }
    
    for( size_t i=0 ; i<data.size() ; ++i )
    {
        assert( data[i].size() == dim );
        for( size_t j=0 ; j<dim ; ++j )
        {
            means[j].first += data[i][j];
            means[j].second += data[i][j] * data[i][j];
        }
    }
    double len = double( data.size() );
    for( size_t j=0 ; j<dim ; ++j )
    {
        double mean = means[j].first / len;
        double sqmean = means[j].second / len;
        means[j].first = mean;
        means[j].second = std::sqrt( sqmean - mean * mean );
    }
    for( size_t i=0 ; i<data.size() ; ++i )
    {
        for( size_t j=0 ; j<dim ; ++j )
        {
            data[i][j] = ( data[i][j] - means[j].first ) / means[j].second;
        }
    }
    
    return means;
}


} // namespace dynsys