/*
  gpcxx/operator/reproduce.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_REPRODUCE_HPP_DEFINED
#define GPCXX_OPERATOR_REPRODUCE_HPP_DEFINED

#include <algorithm>

namespace gpcxx {

template< typename Selector>
class reproduce
{
public:
    
    reproduce( Selector selector ) : m_selector( selector ) { }
    
    template< typename Pop , typename Fitness >
    typename Pop::value_type
    operator()( Pop const& pop , Fitness const& fitness ) const
    {
        return *( m_selector( pop , fitness ) );
    }
    
private:
    
    Selector m_selector;
};

template< typename Selector >
reproduce< Selector > make_reproduce( Selector selector )
{
    return reproduce< Selector >( std::move( selector ) );
}



} // namespace gpcxx


#endif // GPCXX_OPERATOR_REPRODUCE_HPP_DEFINED
