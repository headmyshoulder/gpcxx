/*
 * gpcxx/tree/intrusive_erc_generator.hpp
 * Date: 2015-06-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_INTRUSIVE_ERC_GENERATOR_HPP_INCLUDED
#define GPCXX_TREE_INTRUSIVE_ERC_GENERATOR_HPP_INCLUDED

#include <string>


namespace gpcxx {


template< typename Value , typename Dist >
struct intrusive_erc_generator
{
    using value_type = Value;
    
    intrusive_erc_generator( Dist const& dist )
    : m_dist( dist ) {}
    
    template< typename Rng >
    value_type operator()( Rng& rng ) const
    {
        auto x = m_dist( rng );
        return value_type { [x]( auto const& c , auto const& n ) { return x; } , std::to_string( x ) };
    }
    
private:
    
    Dist m_dist;
};

template< typename Value , typename Dist >
auto make_intrusive_erc_generator( Dist const& dist )
{
    return intrusive_erc_generator< Value , Dist >( dist );
}



} // namespace gpcxx


#endif // GPCXX_TREE_INTRUSIVE_ERC_GENERATOR_HPP_INCLUDED
