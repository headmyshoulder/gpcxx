/*
 * lorenz_reconstruct.cpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "serialize.hpp"

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>


#include <iostream>
#include <fstream>
#include <iterator>

struct lorenz_reconstructed
{
    lorenz_reconstructed( dynsys::individual_type individual , dynsys::norm_type xnorm , dynsys::norm_type ynorm )
    : m_individual( std::move( individual ) ) , m_xnorm( std::move( xnorm ) ) , m_ynorm( std::move( ynorm ) )
    {}
    
    void operator()( dynsys::state_type x , dynsys::state_type& dxdt , double t ) const
    {
        denormalize( x , m_xnorm );
        dxdt[0] = m_individual[0].root()->eval( x );
        dxdt[1] = m_individual[1].root()->eval( x );
        dxdt[2] = m_individual[2].root()->eval( x );
        denormalize( dxdt , m_ynorm );
    }
    
    static void denormalize( dynsys::state_type& x , dynsys::norm_type const& norm )
    {
        assert( x.size() == norm.size() );
        for( size_t i=0 ; i<x.size() ; ++i )
            x[i] = ( x[i] - norm[i].first ) / norm[i].second;
    }
    
    static void normalize( dynsys::state_type& x , dynsys::norm_type const& norm )
    {
        assert( x.size() == norm.size() );
        for( size_t i=0 ; i<x.size() ; ++i )
            x[i] = ( x[i] + norm[i].first ) * norm[i].second;
    }
    
    dynsys::individual_type m_individual;
    dynsys::norm_type m_xnorm;
    dynsys::norm_type m_ynorm;
};

int main( int argc , char** argv )
{
    if( argc != 2 )
    {
        std::cerr << "usage: " << argv[0] << " winner-file" << "\n";
        return -1;
    }
    
    std::ifstream fin { argv[1] };
    
    std::string json_string { std::istream_iterator< char >{ fin } , std::istream_iterator< char >{ } };
    auto winner = dynsys::deserialize_winner( json_string );
    
    std::cout << "Expressions" << std::endl;
    std::cout << gpcxx::polish_string( winner.trees[0] ) << std::endl;
    std::cout << gpcxx::polish_string( winner.trees[1] ) << std::endl;
    std::cout << gpcxx::polish_string( winner.trees[2] ) << std::endl;    
    std::cout << std::endl;
    
    std::cout << "Normalization" << std::endl;
    for( size_t i=0 ; i<dynsys::dim ; ++i )
        std::cout << winner.xnorm[i].first << " " << winner.xnorm[i].second << " " << winner.ynorm[i].first << " " << winner.ynorm[i].second << "\n";
    std::cout << std::endl;
    
    auto sys = lorenz_reconstructed { winner.trees , winner.xnorm , winner.ynorm };
    
    using stepper_type = boost::numeric::odeint::runge_kutta4< dynsys::state_type > ;
    
    dynsys::state_type x {{ 10.0 , 10.0 , 10.0 }};
    stepper_type stepper;
    std::ofstream fout { "reconstruct.dat" };
    boost::numeric::odeint::integrate_const( stepper , sys , x , 0.0 , 100.0 , 0.1 , [&fout]( auto const& x , auto t ) {
        fout << x[0] << " " << x[1] << " " << x[2] << "\n"; } );
    
    std::ofstream fout2 { "functions.dat" };
    for( double x = -20.0 ; x <= 20.0 ; x += 2.0 )
    {
        for( double y = -20.0 ; y <= 20.0 ; y += 2.0 )
        {
            for( double z = -20.0 ; z <= 20.0 ; z += 2.0 )
            {
                dynsys::state_type xx {{ x , y , z }};
                dynsys::state_type f;
                sys( xx , f , 0.0 );
                fout2 << x << " " << y << " " << z << " " << f[0] << " " << f[1] << " " << f[2] << "\n";
            }
        }
    }

    
    
    return 0;
}