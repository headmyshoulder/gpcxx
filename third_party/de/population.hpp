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

#ifndef DE_POPULATION_HPP_INCLUDED
#define DE_POPULATION_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "individual.hpp"


namespace de
{

typedef std::vector< individual_ptr > population_base;

/**
 * A collection of individuals.
 * 
 * @author adrian (12/4/2011)
 */
class population : public population_base
{
public:
	/**
	 * constructs a population object containing uninitialized 
	 * individuals 
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param popSize the population size (number of individuals)
	 * @param varCount the number of variables for each individual 
	 */
	population( size_t popSize, size_t varCount )
	: population_base( popSize )
	{
		assert( popSize > 0 );
		assert( varCount > 0 );

		init(popSize, varCount );
	}

	/**
	 * constructs a population object and initializes its 
	 * individuals by first setting all variables to random values 
	 * within the limits imposed by the corresponding constraints, 
	 * then runs the objective function for each individual to 
	 * calculate the associated cost. 
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param popSize the population size (number of individuals)
	 * @param varCount the number of variables for each individual 
	 * @param constraints constraints to use when initializing the 
	 *  				  individuals
	 */
	population( size_t popSize, size_t varCount, constraints_ptr constraints )
	: population_base( popSize )
	{
		assert( popSize > 0 );
		assert( varCount > 0 );

		init(popSize, varCount );

		for( population::size_type i = 0; i < size(); ++i ) 
			at( i )->init( constraints );
	}

	/**
	 * returns the best individual in a population
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @return individual_ptr 
	 */
	individual_ptr best( bool minimize ) const 
	{
		population::size_type best( 0 );

		for( population::size_type i = 0; i < size(); ++i ) 
			best = at( i )->better_or_equal( at( best ), minimize )? i : best;

		return at( best );
	}

	/**
	 * returns the string representation of a population
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @return std::string string representation of the population
	 */
	std::string to_string()
	{
		std::string str;
		for( population::size_type i = 0; i < size(); ++i ) 
			str += at( i )->to_string();

		return str;
	}

private:
	void init( size_t popSize, size_t varCount )
	{
		for( population_base::size_type i = 0; i < size(); ++i ) 
			operator[]( i ) = boost::make_shared< individual >( varCount );
	}
public:

};

	typedef boost::shared_ptr< population > population_ptr;

}

#endif //DE_POPULATION_HPP_INCLUDED
