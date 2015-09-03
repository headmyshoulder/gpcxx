/*
 * gpcxx/canonic/algebraic_node.hpp
 * Date: 2015-09-02
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_ALGEBRAIC_NODE_HPP_INCLUDED
#define GPCXX_CANONIC_ALGEBRAIC_NODE_HPP_INCLUDED

#include <gpcxx/tree/intrusive_nodes/intrusive_node.hpp>

#include <string>


namespace gpcxx {

// TODO: use intrusive_named_func_node
template< typename Allocator = std::allocator< void* > >
class algebraic_node : public intrusive_node< algebraic_node< Allocator > , Allocator >
{
public:
    
    using allocator_type = Allocator;
    using self_type = algebraic_node< allocator_type >;
    
    
    algebraic_node( std::string const& name , bool commutative , bool associative , bool constant , int precedence )
    : m_name( name )
    , m_commutative( commutative )
    , m_associative( associative )
    , m_constant( constant )
    , m_precedence( precedence ) {}
    
    std::string const& name( void ) const { return m_name; }
    
    std::string& name( void ) { return m_name; }
    
    bool commutative( void ) const { return m_commutative; }
    
    bool associative( void ) const { return m_associative; }
    
    bool constant( void ) const { return m_constant; }
    
    int precedence( void ) const { return m_precedence; }
    
    
    
    
    // Factory functions for default nodes
    static self_type make_commutative_binary_operation( std::string const& name )
    {
        return self_type { name , true , true , false , 20 };
    }
    
    static self_type make_unary_operation( std::string const& name )
    {
        return self_type { name , false , false , false , 10 };
    }
    
    static self_type make_identity_operation( std::string  const& name )
    {
        return self_type { name , false , false , false , 11 };
    }
    
    static self_type make_constant_terminal( std::string const& name )
    {
        return self_type { name , false , false , true , 0 };
    }
    
    static self_type make_variable_terminal( std::string const& name )
    {
        return self_type { name , false , false , false , 1 };
    }

    
    
    
private:
    std::string m_name;
    bool m_commutative;
    bool m_associative;
    bool m_constant;
    int m_precedence;
};

template< typename Allocator >
std::ostream& operator<<( std::ostream& out , algebraic_node< Allocator > const& n )
{
    out << n.name();
    return out;
}

template< typename Allocator >
bool operator==( algebraic_node< Allocator > const& n1 , algebraic_node< Allocator > const& n2 )
{
    return ( n1.precedence() == n2.precedence() ) && ( n1.name() == n2.name() );
}

template< typename Allocator >
bool operator!=( algebraic_node< Allocator > const& n1 , algebraic_node< Allocator > const& n2 )
{
    return ! ( n1 == n2 );
}

template< typename Allocator >
bool operator<( algebraic_node< Allocator > const& n1 , algebraic_node< Allocator > const& n2 )
{
    if( n1.precedence() < n2.precedence() ) return true;
    if( n1.precedence() == n2.precedence() ) return ( n1.name() < n2.name() );
    return false;
}


} // namespace gpcxx


#endif // GPCXX_CANONIC_ALGEBRAIC_NODE_HPP_INCLUDED
