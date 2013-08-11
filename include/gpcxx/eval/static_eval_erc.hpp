/*
  gpcxx/eval/basic_eval_erc.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
  
  ERC = Ephemeral Random Constant
*/


#ifndef GPCXX_EVAL_BASIC_EVAL_ERC_HPP_DEFINED
#define GPCXX_EVAL_BASIC_EVAL_ERC_HPP_DEFINED

#include <gpcxx/util/iterate_until.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>
#include <gpcxx/generate/uniform_symbol_erc.hpp>

#include <boost/variant.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <vector>
#include <stdexcept>



#include <iostream>
using namespace std;



namespace gpcxx {

template< typename Value ,
          typename Symbol ,
          typename EvalContext ,
          typename Erc ,
          typename TerminalAttributes ,
          typename UnaryAttributes ,
          typename BinaryAttributes >
class static_eval_erc
{    
    typedef static_eval_erc< Value , Symbol , EvalContext , Erc , TerminalAttributes , UnaryAttributes , BinaryAttributes > self_type;
    
public:
    
    typedef Value value_type;
    typedef Symbol symbol_type;    
    typedef EvalContext eval_context_type;
    typedef Erc erc_type;
    typedef TerminalAttributes terminal_attribtes_type;
    typedef UnaryAttributes unary_attributes_type;
    typedef BinaryAttributes binary_attribtes_type;
    
    typedef typename std::remove_reference< typename boost::fusion::result_of::at_c< erc_type , 1 >::type >::type erc_dist_type;
    
    typedef boost::variant< value_type , symbol_type > node_attribute_type;
    
    
    typedef uniform_symbol_erc< symbol_type , value_type , erc_dist_type , node_attribute_type > symbol_erc_distribution_type;
    typedef uniform_symbol< symbol_type > symbol_distribution_type;

    
    
    static_eval_erc( erc_type erc , terminal_attribtes_type const& terminals , unary_attributes_type const& unaries , binary_attribtes_type const& binaries )
    : m_erc( erc ) , m_terminals( terminals ) , m_unaries( unaries ) , m_binaries( binaries ) { }
    
    template< typename Tree >
    value_type operator()( Tree const& tree , eval_context_type const& context ) const
    {
        return eval_cursor( tree.root() , context );
    }
    
    std::vector< symbol_type > get_terminal_symbols( void ) const
    {
        return get_symbols( m_terminals );
    }
    
    std::vector< symbol_type > get_unary_symbols( void ) const
    {
        return get_symbols( m_unaries );
    }
    
    std::vector< symbol_type > get_binary_symbols( void ) const
    {
        return get_symbols( m_binaries );
    }
    
    
    
    symbol_erc_distribution_type get_terminal_symbol_distribution( void ) const
    {
        return symbol_erc_distribution_type(
                   get_terminal_symbols() ,
                   boost::fusion::at_c< 0 >( m_erc ) ,
                   boost::fusion::at_c< 1 >( m_erc ) );
    }

    symbol_distribution_type get_unary_symbol_distribution( void ) const
    {
        return symbol_distribution_type( get_unary_symbols() );
    }
     
    symbol_distribution_type get_binary_symbol_distribution( void ) const
    {
        return symbol_distribution_type( get_binary_symbols() );
    }
   
private:
    
    struct symbol_filler
    {
        std::vector< symbol_type > &m_symbols;
        symbol_filler( std::vector< symbol_type > &symbols ) : m_symbols( symbols ) { }
        
        template< typename Entry >
        void operator()( Entry const& e ) const
        {
            m_symbols.push_back( boost::fusion::front( e ) );
        }
    };
    
    template< typename Symbols >
    std::vector< symbol_type > get_symbols( Symbols const& s ) const
    {
        std::vector< symbol_type > ret;
        boost::fusion::for_each( s , symbol_filler( ret ) );
        return ret;
    }
    
    template< typename Cursor >
    struct evaluator_base
    {
        self_type const& m_self;
        value_type &m_result;
        eval_context_type const& m_context;
        Cursor const& m_cursor;
        
        evaluator_base( self_type const& self , value_type &result , eval_context_type const& context , Cursor const& cursor )
        : m_self( self ) , m_result( result ) , m_context( context ) , m_cursor( cursor ) { }
    };
        
    
    template< typename Cursor >
    struct terminal_evaluator : evaluator_base< Cursor >
    {
        terminal_evaluator( self_type const& self , value_type &result , eval_context_type const& context , Cursor const& cursor )
        : evaluator_base< Cursor >( self , result , context , cursor ) { }
        
        template< typename Entry >
        bool operator()( Entry const &e ) const
        {
            if( boost::fusion::front( e ) == boost::get< symbol_type >( *(this->m_cursor) ) )
            {
                this->m_result = boost::fusion::at_c< 1 >( e )( this->m_context );
                return true;
            }
            return false;
        }
    };
    
    template< typename Cursor >
    struct unary_evaluator : evaluator_base< Cursor >
    {
        unary_evaluator( self_type const& self , value_type &result , eval_context_type const& context , Cursor const& cursor )
        : evaluator_base< Cursor >( self , result , context , cursor ) { }
        
        template< typename Entry >
        bool operator()( Entry const &e ) const
        {
            if(  boost::fusion::front( e ) == boost::get< symbol_type >( *(this->m_cursor) ) )
            {
                value_type val = this->m_self.eval_cursor( this->m_cursor.children( 0 ) , this->m_context );
                this->m_result = boost::fusion::at_c< 1 >( e )( val );
                return true;
            }
            return false;
        }
    };
    
    template< typename Cursor >
    struct binary_evaluator : evaluator_base< Cursor >
    {
        binary_evaluator( self_type const& self , value_type &result , eval_context_type const& context , Cursor const& cursor )
        : evaluator_base< Cursor >( self , result , context , cursor ) { }
        
        template< typename Entry >
        bool operator()( Entry const &e ) const
        {
            if( boost::fusion::front( e ) == boost::get< symbol_type >( *(this->m_cursor) ) )
            {
                value_type val1 = this->m_self.eval_cursor( this->m_cursor.children( 0 ) , this->m_context );
                value_type val2 = this->m_self.eval_cursor( this->m_cursor.children( 1 ) , this->m_context );
                this->m_result = boost::fusion::at_c< 1 >( e )( val1 , val2 );
                return true;
            }
            return false;
        }
    };

    
    template< typename Cursor >
    value_type eval_cursor( Cursor cursor , eval_context_type const& context ) const
    {
        value_type result = 0.0;
        bool found = true;
        
        if( cursor.size() == 0 )
        {
            node_attribute_type const& v = *cursor;
            if( v.which() == 1 )
            {
                found &= gpcxx::iterate_until( m_terminals , terminal_evaluator< Cursor >( *this , result , context , cursor ) );
            }
            else 
            {
                result = boost::get< value_type >( v );
                found = true;
            }
        }
        else if( cursor.size() == 1 )
            found &= gpcxx::iterate_until( m_unaries , unary_evaluator< Cursor >( *this , result , context , cursor ) );
        else if( cursor.size() == 2 ) 
            found &= gpcxx::iterate_until( m_binaries , binary_evaluator< Cursor >( *this , result , context , cursor ) );
        else
            throw std::runtime_error( "basic_eval::eval_cursor : Node with arity higher then two node supported!" );
        
        if( !found ) throw std::runtime_error( "basic_eval::eval_cursor : No rule found!" );
        
        return result;
    }


    erc_type m_erc;
    terminal_attribtes_type m_terminals;
    unary_attributes_type m_unaries;
    binary_attribtes_type m_binaries;
    

    static_assert( boost::fusion::traits::is_sequence< Erc >::value , "Erc must be a Boost.Fusion sequence" );
    static_assert( boost::fusion::traits::is_sequence< TerminalAttributes >::value , "TerminalAttributes must be a Boost.Fusion sequence" );
    static_assert( boost::fusion::traits::is_sequence< UnaryAttributes >::value , "UnaryAttributes must be a Boost.Fusion sequence" );
    static_assert( boost::fusion::traits::is_sequence< BinaryAttributes >::value , "BinaryAttributes must be a Boost.Fusion sequence" );
};


template< typename Value , typename Symbol , typename EvalContext ,
          typename Erc , typename TerminalAttributes, typename UnaryAttributes , typename BinaryAttributes >
static_eval_erc< Value , Symbol , EvalContext , Erc , TerminalAttributes , UnaryAttributes , BinaryAttributes >
make_static_eval_erc( Erc const &erc , TerminalAttributes const& terminals , UnaryAttributes const& unaries , BinaryAttributes const& binaries )
{
    return static_eval_erc< Value , Symbol , EvalContext , Erc , TerminalAttributes , UnaryAttributes , BinaryAttributes >( erc , terminals , unaries , binaries );
}



} // namespace gpcxx


#endif // GPCXX_EVAL_BASIC_EVAL_ERC_HPP_DEFINED
