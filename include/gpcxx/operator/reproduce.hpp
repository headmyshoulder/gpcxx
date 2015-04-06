/*
  gpcxx/operator/reproduce.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GPCXX_OPERATOR_REPRODUCE_HPP_DEFINED
#define GPCXX_OPERATOR_REPRODUCE_HPP_DEFINED

#include <vector>
#include <cassert>

namespace gpcxx {

template< typename Selector>
class reproduce
{
public:
    
    reproduce( Selector selector ) : m_selector( selector ) { }
    
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::value_type >
    operator()( Pop const& pop , Fitness const& fitness ) const
    {
        std::vector< typename Pop::value_type > nodes( 1 );
        nodes[0] = *( m_selector( pop , fitness ) );
        return nodes;
    }
   
    template< typename Pop , typename Fitness >
    std::vector< typename Pop::const_iterator >
    selection( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::const_iterator > s( 1 );
        s[0] = m_selector( pop , fitness );
        return s;
    }
    
    template< typename Selection >
    std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type >
    operation( Selection const& selection )
    {
        assert( selection.size() == 1 );
        std::vector< typename std::iterator_traits< typename Selection::value_type >::value_type > nodes( 1 );
        nodes[0] = *( selection[0] );
        return nodes;        
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
