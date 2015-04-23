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

#include <iostream>
#include <vector>

int main( int argc , char** argv )
{
    std::vector< double > fitness = { 2.0 , 10.0 , 40.0 , 0.5 , 0.00001 };
    
    {
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
        //[ adjusted_fitness_example
//         auto a = gpcxx::adjusted_fitness_copy( fitness );
//         auto av = gpcxx::adjusted_fitness_view( fitness );
//         vector< double > af;
//         gpcxx::adjusted_fitness_inplace( fitness , af );
        //]
    }

    
    return 0;
}