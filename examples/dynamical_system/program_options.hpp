/*
 * gpcxx/examples/dynamical_system/program_options.hpp
 * Date: 2016-02-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_PROGRAM_OPTIONS_HPP_INCLUDED
#define GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_PROGRAM_OPTIONS_HPP_INCLUDED

#include <boost/program_options.hpp>

#include <string>
#include <utility>


namespace dynsys {


inline auto get_options( void )
{
    namespace po = boost::program_options;
    po::options_description options( "Options" );
    options.add_options()
    ( "normalize" , po::value< bool >() , "normalize the input data" )
    ;
    return options;
}

inline auto get_positional_options( void )
{
    namespace po = boost::program_options;
    po::options_description hidden( "Positional arguments" );
    hidden.add_options()
    ( "evolution" , po::value< std::string >() , "outfile for the evolution" )
    ( "result" , po::value< std::string >() , "outfile for the result" )
    ( "winner" , po::value< std::string >() , "outfile for the winner" )
    ;
    
    po::positional_options_description positional_options;
    positional_options.add( "evolution", 1 ).add( "result", 1 ).add( "winner" , 1 );
    
    return std::make_pair( positional_options , hidden );
}

} // namespace dynsys


#endif // GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_PROGRAM_OPTIONS_HPP_INCLUDED
