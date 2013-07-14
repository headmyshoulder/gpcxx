/*
  gp/eval/basic_eval.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_EVAL_BASIC_EVAL_HPP_DEFINED
#define GP_EVAL_BASIC_EVAL_HPP_DEFINED


#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/front.hpp>

#include <stdexcept>



namespace gp {

    
    
template< typename Value ,
          typename Context ,
          typename NodeAttribute ,
          typename TerminalAttributes,
          typename UnaryAttributes ,
          typename BinaryAttributes >
class basic_eval
{
    static_assert( boost::fusion::traits::is_sequence< TerminalAttributes >::value , "TerminalAttributes must be a Boost.Fusion sequence" );
    static_assert( boost::fusion::traits::is_sequence< UnaryAttributes >::value , "UnaryAttributes must be a Boost.Fusion sequence" );
    static_assert( boost::fusion::traits::is_sequence< BinaryAttributes >::value , "BinaryAttributes must be a Boost.Fusion sequence" );
    
    typedef basic_eval< Value , Context , NodeAttribute , TerminalAttributes , UnaryAttributes , BinaryAttributes > self_type;
    
    
    
public:
    
    typedef Value value_type;
    typedef Context context_type;
    typedef NodeAttribute node_attribute_type;
    typedef TerminalAttributes terminal_attribtes_type;
    typedef UnaryAttributes unary_attributes_type;
    typedef BinaryAttributes binary_attribtes_type;
    
    
    basic_eval( terminal_attribtes_type const& terminals , unary_attributes_type const& unaries , binary_attribtes_type const& binaries )
    : m_terminals( terminals ) , m_unaries( unaries ) , m_binaries( binaries ) { }
    
    template< typename Tree >
    value_type operator()( Tree const& tree , context_type const& context ) const
    {
        return eval_cursor( tree.root() , context );
    }
   
private:
    
    template< typename Cursor >
    struct evaluator_base
    {
        self_type const& m_self;
        value_type &m_result;
        context_type const& m_context;
        Cursor const& m_cursor;
        bool &m_found;
        
        evaluator_base( self_type const& self , value_type &result , context_type const& context , Cursor const& cursor , bool &found )
        : m_self( self ) , m_result( result ) , m_context( context ) , m_cursor( cursor ) , m_found( found ) { }
    };
        
    
    template< typename Cursor >
    struct terminal_evaluator : evaluator_base< Cursor >
    {
        terminal_evaluator( self_type const& self , value_type &result , context_type const& context , Cursor const& cursor , bool &found )
        : evaluator_base< Cursor >( self , result , context , cursor , found ) { }
        
        template< typename Entry >
        void operator()( Entry const &e ) const
        {
            if( boost::fusion::front( e ) == *(this->m_cursor) )
            {
                this->m_result = boost::fusion::at_c< 1 >( e )( this->m_context );
                this->m_found = true;
            }
        }
    };
    
    template< typename Cursor >
    struct unary_evaluator : evaluator_base< Cursor >
    {
        unary_evaluator( self_type const& self , value_type &result , context_type const& context , Cursor const& cursor , bool &found )
        : evaluator_base< Cursor >( self , result , context , cursor , found ) { }
        
        template< typename Entry >
        void operator()( Entry const &e ) const
        {
            if(  boost::fusion::front( e ) == *(this->m_cursor) )
            {
                value_type val = this->m_self.eval_cursor( this->m_cursor.children( 0 ) , this->m_context );
                this->m_result = boost::fusion::at_c< 1 >( e )( val );
                this->m_found = true;
            }
        }
    };
    
    template< typename Cursor >
    struct binary_evaluator : evaluator_base< Cursor >
    {
        binary_evaluator( self_type const& self , value_type &result , context_type const& context , Cursor const& cursor , bool &found )
        : evaluator_base< Cursor >( self , result , context , cursor , found ) { }
        
        template< typename Entry >
        void operator()( Entry const &e ) const
        {
            if( boost::fusion::front( e ) == *(this->m_cursor) )
            {
                value_type val1 = this->m_self.eval_cursor( this->m_cursor.children( 0 ) , this->m_context );
                value_type val2 = this->m_self.eval_cursor( this->m_cursor.children( 1 ) , this->m_context );
                this->m_result = boost::fusion::at_c< 1 >( e )( val1 , val2 );
                this->m_found = true;                
            }
        }
    };

    
    template< typename Cursor >
    value_type eval_cursor( Cursor cursor , context_type const& context ) const
    {
        value_type result = 0.0;
        bool found = false;
        
        if( cursor.size() == 0 )
            boost::fusion::for_each( m_terminals , terminal_evaluator< Cursor >( *this , result , context , cursor , found ) );
        else if( cursor.size() == 1 )
            boost::fusion::for_each( m_unaries , unary_evaluator< Cursor >( *this , result , context , cursor , found ) );
        else if( cursor.size() == 2 ) 
            boost::fusion::for_each( m_binaries , binary_evaluator< Cursor >( *this , result , context , cursor , found ) );
        else
            throw std::runtime_error( "basic_eval::eval_cursor : Node with arity higher then two node supported!" );
        
        if( !found ) throw std::runtime_error( "basic_eval::eval_cursor : No rule found!" );
        
        return result;
    }

    
    terminal_attribtes_type m_terminals;
    unary_attributes_type m_unaries;
    binary_attribtes_type m_binaries;
};


template< typename Value , typename Context , typename NodeAttribute ,
          typename TerminalAttributes, typename UnaryAttributes , typename BinaryAttributes >
basic_eval< Value , Context , NodeAttribute , TerminalAttributes , UnaryAttributes , BinaryAttributes >
make_basic_eval( TerminalAttributes const& terminals , UnaryAttributes const& unaries , BinaryAttributes const& binaries )
{
    return basic_eval< Value , Context , NodeAttribute , TerminalAttributes , UnaryAttributes , BinaryAttributes >( terminals , unaries , binaries );
}


} // namespace gp


#endif // GP_EVAL_BASIC_EVAL_HPP_DEFINED
