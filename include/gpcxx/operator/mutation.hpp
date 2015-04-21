
/*
 * mutation.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_OPERATOR_MUTATION_H_INCLUDED
#define GPCXX_OPERATOR_MUTATION_H_INCLUDED

#include <gpcxx/operator/detail/operator_base.hpp>

#include <utility>
#include <vector>
#include <cassert>


namespace gpcxx {

template< typename Strategy , typename Selector >
class mutation : public detail::operator_base< Strategy::arity >
{
public:
    
    mutation( Strategy strategy , Selector selector )
    : m_strategy( std::move( strategy ) ) , m_selector( std::move( selector ) ) { }

    template< typename Pop , typename Fitness >
    std::vector< typename Pop::value_type >
    operator()( Pop const& pop , Fitness const& fitness )
    {
        std::vector< typename Pop::value_type > nodes( 1 );
        nodes[0] = *( m_selector( pop , fitness ) );
        if( ! nodes[0].empty() )
            m_strategy( nodes[0] );
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
        nodes[ 0 ] = *( selection[0] );
        if( ! nodes[0].empty() )
            m_strategy( nodes[0] );
        return nodes;
    }


private:

    Strategy m_strategy;
    Selector m_selector;
};

template< typename Strategy , typename Selector >
mutation< Strategy , Selector > make_mutation( Strategy strategy , Selector selector )
{
    return mutation< Strategy , Selector >( std::move( strategy ) , std::move( selector ) );
}



} // namespace gpcxx

#endif // GPCXX_OPERATOR_MUTATION_H_INCLUDED
