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

#include <gpcxx/tree/cursor_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>


namespace gpcxx {


template< typename Cursor , typename OrderPolicy >
class iterator_base : public boost::iterator_facade< 
    iterator_base< Cursor , OrderPolicy > ,
    typename cursor_value< Cursor >::type ,
    boost::bidirectional_traversal_tag >
{
    struct enabler {};
    
    using reference_type = typename cursor_value< Cursor >::type& ;
    using iterator_facade_ = boost::iterator_facade< 
        iterator_base< Cursor , OrderPolicy > ,
        typename cursor_value< Cursor >::type ,
        boost::bidirectional_traversal_tag >;
    
public:
    
    iterator_base( void ) : m_cursor {} {}
    
    iterator_base( Cursor c ) : m_cursor { c } {}
    
    template <class OtherCursor>
    iterator_base( iterator_base< OtherCursor , OrderPolicy > const& other , typename boost::enable_if<
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
        OrderPolicy::successor( m_cursor );
    }
    
    void decrement( void )
    {
        OrderPolicy::predecessor( m_cursor );
    }
    
    reference_type dereference( void ) const
    {
        return *m_cursor;
    }
    
    template< typename OtherCursor >
    bool equal( iterator_base< OtherCursor , OrderPolicy > const& other ) const
    {
        return this->m_cursor == other.m_cursor;
    }
    
    Cursor m_cursor;
};









} // namespace gpcxx


#endif // GPCXX_TREE_ITERATOR_ITERATOR_BASE_HPP_INCLUDED
