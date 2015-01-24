/*
 * gpcxx/tree/iterator/iterator_base.hpp
 * Date: 2015-01-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_ITERATOR_ITERATOR_BASE_HPP_INCLUDED
#define GPCXX_TREE_ITERATOR_ITERATOR_BASE_HPP_INCLUDED

#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>


namespace gpcxx {


template< typename Cursor , typename Order >
class iterator_base : public boost::iterator_facade< 
    iterator_base< Cursor , Order > ,
    typename cursor_value< Cursor >::type ,
    boost::bidirectional_traversal_tag >
{
    struct enabler {};
    
    using reference_type = typename cursor_value< Cursor >::type& ;
    using iterator_facade_ = boost::iterator_facade< 
        iterator_base< Cursor , Order > ,
        typename cursor_value< Cursor >::type ,
        boost::bidirectional_traversal_tag >;
    
public:
    
    iterator_base( void ) : m_cursor {} {}
    
    iterator_base( Cursor c ) : m_cursor { c } {}
    
    template <class OtherCursor>
    iterator_base( iterator_base< OtherCursor , Order > const& other , typename boost::enable_if<
            boost::is_convertible< OtherCursor , Cursor > , enabler >::type = enabler() )
      : m_cursor { other.m_cursor } {}
      
    operator Cursor() 
    {
        return m_cursor;
    }
    
private:
    
    friend class boost::iterator_core_access;
    
    void increment( void )
    {
        Order::successor( m_cursor );
    }
    
    void decrement( void )
    {
        Order::predecessor( m_cursor );
    }
    
    reference_type dereference( void ) const
    {
        return *m_cursor;
    }
    
    template< typename OtherCursor >
    bool equal( iterator_base< OtherCursor , Order > const& other ) const
    {
        return this->m_cursor == other.m_cursor;
    }
    
    Cursor m_cursor;
};


struct inorder_policy
{
    template< typename Cursor >
    static void successor( Cursor &c )
    {
        
    }
    
    template< typename Cursor >
    static void predecessor( Cursor &c )
    {
    }
    
    template< typename Cursor >
    static void first( Cursor &c )
    {
        while( c.size() != 0 )
        {
            c = c.children( 0 );
        }
    }
    
    template< typename Cursor >
    static void last( Cursor &c )
    {
    }
};

template< typename Cursor >
using inorder_iterator = iterator_base< Cursor , inorder_policy >;

template< typename Cursor >
inorder_iterator< Cursor > begin_inorder( Cursor c )
{
    inorder_policy::first( c );
    return inorder_iterator< Cursor > { c };
}

template< typename Cursor >
inorder_iterator< Cursor > end_inorder( Cursor c )
{
    inorder_policy::last( c );
    return inorder_iterator< Cursor > { c };
}




} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_ITERATOR_BASE_HPP_INCLUDED


template<class T >
struct C;
template<class T >
struct C;