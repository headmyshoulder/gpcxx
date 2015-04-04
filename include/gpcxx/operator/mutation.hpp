
/*
 * mutation.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED

#include <utility>


namespace gpcxx {

template< typename Strategy , typename Selector >
class mutation
{
public:

    mutation( Strategy strategy , Selector selector )
    : m_strategy( std::move( strategy ) ) , m_selector( std::move( selector ) ) { }

    template< typename Pop , typename Fitness >
    typename Pop::value_type
    operator()( Pop const& pop , Fitness const& fitness )
    {
        auto t = *( m_selector( pop , fitness ) );
        if( ! t.empty() )
            m_strategy( t );
        return t;
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

#endif // MUTATION_H_INCLUDED
