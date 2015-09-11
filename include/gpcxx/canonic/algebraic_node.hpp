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
#include <functional>
#include <utility>


namespace gpcxx {


template< typename Res , typename Context, typename Allocator = std::allocator< void* > >
class algebraic_node : public gpcxx::intrusive_node< algebraic_node< Res , Context , Allocator > , Allocator >
{
public:
    
    using allocator_type = Allocator;
    using result_type = Res;
    using context_type = Context;
    using self_type = algebraic_node< result_type , context_type , allocator_type >;
    using base_type = intrusive_node< self_type , allocator_type >;
    using func_type = std::function< result_type( context_type& , self_type const& ) >;
    
    algebraic_node( func_type f , std::string name , bool constant , int precedence )
    : m_func( std::move( f ) )
    , m_name( std::move( name ) )
    , m_constant( constant )
    , m_precedence( precedence ) {}
    
    result_type eval( context_type & context ) const
    {
        return m_func( context , *this );
    }
    
    std::string const& name( void ) const
    {
        return m_name;
    }
    
    std::string& name( void )
    {
        return m_name;
    }
    
    bool constant( void ) const { return m_constant; }
    
    int precedence( void ) const { return m_precedence; }
    

    static self_type make_binary_operation( func_type t , std::string name )
    {
        return self_type { std::move( t ) , std::move( name ) , false , 20 };
    }
    
    static self_type make_unary_operation( func_type t , std::string name )
    {
        return self_type { std::move( t ) , std::move( name ) , false , 10 };
    }
    
    static self_type make_identity_operation( func_type t , std::string  name )
    {
        return self_type { std::move( t ) , std::move( name ) , false , 11 };
    }
    
    static self_type make_constant_terminal( func_type t , std::string name )
    {
        return self_type { std::move( t ) , std::move( name ) , true , 0 };
    }
    
    static self_type make_variable_terminal( func_type t , std::string name )
    {
        return self_type { std::move( t ) , std::move( name ) , false , 1 };
    }

    
    
    
private:
    
    func_type m_func;
    std::string m_name;
    bool m_commutative;
    bool m_associative;
    bool m_constant;
    int m_precedence;
};

template< typename Res , typename Context , typename Allocator >
std::ostream& operator<<( std::ostream& out , algebraic_node< Res , Context , Allocator > const& n )
{
    out << n.name();
    return out;
}

template< typename Res , typename Context , typename Allocator >
bool operator==( algebraic_node< Res , Context , Allocator > const& n1 , algebraic_node< Res , Context , Allocator > const& n2 )
{
    return ( n1.precedence() == n2.precedence() ) && ( n1.name() == n2.name() );
}

template< typename Res , typename Context , typename Allocator >
bool operator!=( algebraic_node< Res , Context , Allocator > const& n1 , algebraic_node< Res , Context , Allocator > const& n2 )
{
    return ! ( n1 == n2 );
}

template< typename Res , typename Context , typename Allocator >
bool operator<( algebraic_node< Res , Context , Allocator > const& n1 , algebraic_node< Res , Context , Allocator > const& n2 )
{
    if( n1.precedence() > n2.precedence() ) return true;
    if( n1.precedence() == n2.precedence() ) return ( n1.name() < n2.name() );
    return false;
}


} // namespace gpcxx


#endif // GPCXX_CANONIC_ALGEBRAIC_NODE_HPP_INCLUDED
