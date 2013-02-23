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

#ifndef DE_LISTENER_HPP_INCLUDED
#define DE_LISTENER_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace de
{

/**
 * Abstract base class for user defined listeners. 
 *  
 * A Listener class will receive notifications when certain 
 * significan events happen during an optimization session 
 *  
 * These events can be used to dispaly the current status, 
 * diagnostic information, etc 
 *  
 * * The user will derive concrete listener classes from this 
 * class, which will do something useful with the received 
 * events 
 * 
 * @author adrian (12/1/2011)
 */
class listener
{
public:
	virtual ~listener() {}

	/**
	 * called at the start of the optimization process
	 * 
	 * @author adrian (12/4/2011)
	 */
	virtual void start() = 0;
	/**
	 * called at the end of the optimization process 
	 *  
	 * this function is called even if the optimization process 
	 * ends with an exception 
	 * 
	 * @author adrian (12/4/2011)
	 */
	virtual void end() = 0;
	/**
	 * called if an exception was thrown during the optimization 
	 * process, and signals an error 
	 * 
	 * @author adrian (12/4/2011)
	 */
	virtual void error() = 0;
	/**
	 * called at the start of each generation
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 */
	virtual void startGeneration( size_t genCount ) = 0;
	/**
	 * called at the end of each generation, unless an exception is 
	 * thrown before reaching the end of the iteration 
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 * @param bestIndGen 
	 * @param bestInd 
	 */
	virtual void endGeneration( size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd ) = 0;
	/**
	 * called before the selection starts
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 */
	virtual void startSelection( size_t genCount ) = 0;
	/**
	 * called after the selection has been performed
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 */
	virtual void endSelection( size_t genCount ) = 0;
	/**
	 * called before starting the objective function processing
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 */
	virtual void startProcessors( size_t genCount ) = 0;
	/**
	 * called after the objective function processing has been 
	 * completed 
	 * 
	 * @author adrian (12/4/2011)
	 * 
	 * @param genCount 
	 */
	virtual void endProcessors( size_t genCount ) = 0;
};

/**
 * A smart pointer to a Listener
 */
typedef boost::shared_ptr< listener > listener_ptr;

/**
 * A concrete Listener that ignores all received events
 * 
 * @author adrian (12/1/2011)
 */
class null_listener : public listener
{
public:
	virtual void start(){}
	virtual void end(){}
	virtual void error(){}
	virtual void startGeneration( size_t genCount ){}
	virtual void endGeneration( size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd ){}
	virtual void startSelection( size_t genCount ){}
	virtual void endSelection( size_t genCount ){}
	virtual void startProcessors( size_t genCount ){}
	virtual void endProcessors( size_t genCount ){}
};

}

#endif //DE_LISTENER_HPP_INCLUDED
