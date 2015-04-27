/*
 * fitness.cpp
 * Date: 2015-04-04
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/eval/adjusted_fitness.hpp>
#include <gpcxx/eval/normalized_fitness.hpp>
#include <gpcxx/eval/hits.hpp>
#include <gpcxx/util/macros.hpp>

#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include <iostream>
#include <vector>

int main( int argc , char** argv )
{
    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ fitness_simple_example
        auto adjusted_fitness = gpcxx::adjusted_fitness_copy( fitness );
        auto normalized_fitness = gpcxx::normalized_fitness_copy( adjusted_fitness );
        auto hits = gpcxx::hits( adjusted_fitness , 0.001 );
        //]
        for( auto n : normalized_fitness ) std::cout << n << std::endl;
        for( auto h : hits ) std::cout << h << std::endl;
        std::cout << std::endl;
    }
    
    
    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ fitness_simple_views_1
        auto adjusted_fitness = gpcxx::adjusted_fitness_view( fitness );
        auto normalized_fitness = gpcxx::normalized_fitness_copy( adjusted_fitness );
        auto hits = gpcxx::hits( adjusted_fitness , 0.001 );
        //]
        for( auto n : normalized_fitness ) std::cout << n << std::endl;
        for( auto h : hits ) std::cout << h << std::endl;
        std::cout << std::endl;
    }
    
    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ fitness_simple_views_2
        auto adjusted_fitness = gpcxx::adjusted_fitness_view( fitness );
        auto normalized_fitness = gpcxx::normalized_fitness_view( adjusted_fitness );
        auto hits = gpcxx::hits_view( adjusted_fitness , 0.001 );
        //]
        for( auto n : normalized_fitness ) std::cout << n << std::endl;
        for( auto h : hits ) std::cout << h << std::endl;
        std::cout << std::endl;
    }
    
    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ adjusted_fitness_example
        // computes the adjusted fitness and stores the result in a
        std::vector< double > a( fitness.size() , 0.0 );
        gpcxx::adjusted_fitness( fitness , a );
        
        // returns a vector containing the adjusted fitness
        auto ax = gpcxx::adjusted_fitness_copy( fitness );

        // returns a view containing the adjusted fitness,
        // hence av is not a container but a range which computes
        // the adjusted fitness only if it is needed
        auto av = gpcxx::adjusted_fitness_view( fitness );

        // computes the adjusted fitness inplace
        gpcxx::adjusted_fitness_inplace( fitness );
        //]
        GPCXX_UNUSED_VAR( ax );
        GPCXX_UNUSED_VAR( av );
    }
    
    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ normalized_fitness_example
        // computes the normalized fitness and stores the result in n
        std::vector< double > n( fitness.size() , 0.0 );
        gpcxx::normalized_fitness( fitness , n );

        // returns a vector containing the normalized fitness
        auto nx = gpcxx::normalized_fitness_copy( fitness );

        // returns a view containing the normalized fitness,
        // hence nv is not a container but a range which computes
        // the normalized fitness only if it is needed
        auto nv = gpcxx::normalized_fitness_view( fitness );

        // computes the adjusted fitness inplace
        gpcxx::normalized_fitness_inplace( fitness );
        //]
        GPCXX_UNUSED_VAR( nx );
        GPCXX_UNUSED_VAR( nv );
    }

    {
        std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 , 1.0 };
        //[ hits_example
        // computes the hits and stores the result in n
        std::vector< bool > h( fitness.size() , 0.0 );
        double tolerance = 1.0e-3;
        gpcxx::hits( fitness , h , tolerance );
        

        // returns a vector containing the hits
        auto hx = gpcxx::hits( fitness , tolerance );

        // returns a view containing the hits
        auto hv = gpcxx::hits_view( fitness , tolerance );
        
        // compute the number of hits
        auto number_of_hits = boost::count( hv , true );
         //]
        GPCXX_UNUSED_VAR( hx );
        GPCXX_UNUSED_VAR( number_of_hits );
    }



    
    return 0;
}