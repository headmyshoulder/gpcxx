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

#ifndef DE_TERMINATION_STRATEGY_HPP_INCLUDED
#define DE_TERMINATION_STRATEGY_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace de
{

/**
 * Abstract base class defining the interface used by a concrete 
 * Termination Strategy class 
 *  
 * A termination strategy defines the logic used to stop the 
 * optimization process. Can be as simple as a generation 
 * counter that exits when the max number of generations has 
 * been reached, or can implement more complex algorithms that 
 * try to minimize the number of execution steps by 
 * determinining when a reasonable best value has been reached. 
 * 
 * @author adrian (12/1/2011)
 */
class termination_strategy
{
public:
	virtual ~termination_strategy(){}

	/**
	 * 
	 * 
	 * @author adrian (12/1/2011)
	 * 
	 * @param best The best individual so far
	 * @param genCount generation number
	 * 
	 * @return bool return true to continue, or false to stop the 
	 *  	   optimization process
	 */
	virtual bool event( individual_ptr best, size_t genCount ) = 0;
};

/**
 * A smart pointer to a TerminationStrategy
 */
typedef boost::shared_ptr< termination_strategy > termination_strategy_ptr;

/**
 * Basic implementation of a Termination Strategy: stop the 
 * optimization process if a maximum number of generations has 
 * been reached 
 * 
 * @author adrian (12/1/2011)
 */
class max_gen_termination_strategy : public termination_strategy
{
private:
	const size_t m_maxGen;
public:
	/**
	 * constructs a max_gen_termination_strategy object
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param maxGen maximum number of generations after which the 
	 *  			 optimization stops
	 */
	max_gen_termination_strategy( size_t maxGen )
		: m_maxGen( maxGen )
	{
	}

	virtual bool event( individual_ptr best, size_t genCount )
	{
		return genCount < m_maxGen;
	}
};

}

#endif //DE_TERMINATION_STRATEGY_HPP_INCLUDED
