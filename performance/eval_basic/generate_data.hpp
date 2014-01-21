/*
 * gpcxx/performance/eval_basic/generate_data.hpp
 * Date: 2014-01-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_EVAL_BASIC_GENERATE_DATA_HPP_INCLUDED
#define GPCXX_PERFORMANCE_EVAL_BASIC_GENERATE_DATA_HPP_INCLUDED


template< typename SequenceContainer , typename Value >
void generate_test_data( SequenceContainer &x1 , SequenceContainer &x2 , SequenceContainer &x3 , Value rmin , Value rmax , Value stepsize )
{
    x1.clear() ;
    x2.clear() ;
    x3.clear();
    
    for( Value xx = rmin ; xx <= rmax ; xx += stepsize )
    {
        for( Value yy = rmin ; yy <= rmax ; yy += stepsize )
        {
            for( Value zz = rmin ; zz <= rmax ; zz += stepsize )
            {
                x1.push_back( xx );
                x2.push_back( yy );
                x3.push_back( zz );
            }
        }
    }
}




#endif // GPCXX_PERFORMANCE_EVAL_BASIC_GENERATE_DATA_HPP_INCLUDED
