/*
 * Copyright (c) 2011 Adrian Michel
 * http://www.amichel.com
 *
 * Permission to use, copy, modify, distribute and sell this 
 * software and its documentation for any purpose is hereby 
 * granted without fee, provided that both the above copyright 
 * notice and this permission notice appear in all copies and in 
 * the supporting documentation. 
 *  
 * This library is distributed in the hope that it will be 
 * useful. However, Adrian Michel makes no representations about
 * the suitability of this software for any purpose.  It is 
 * provided "as is" without any express or implied warranty. 
 * 
 * Should you find this library useful, please email 
 * info@amichel.com with a link or other reference 
 * to your work. 
*/

#ifndef DE_RANDOM_GENERATOR_HPP_INCLUDED
#define DE_RANDOM_GENERATOR_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/math/special_functions/round.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace de
{

inline double genrand( double min = 0, double max = 1 )
{
	static boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> dist( min, max );
	boost::variate_generator<boost::random::mt19937&, boost::random::uniform_real_distribution< double > > value(gen, dist);

	return value();
}

inline int genintrand( double min, double max, bool upperexclusive = false )
{
	assert( min < max );
	int ret = 0;
	do ret = boost::math::round( genrand( min, max ) ); while( ret < min || ret > max || upperexclusive && ret == max );
	return ret; 
}

}

#endif //DE_RANDOM_GENERATOR_HPP_INCLUDED
