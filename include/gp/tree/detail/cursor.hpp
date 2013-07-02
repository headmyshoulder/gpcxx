/*
  gp/tree/detail/cursor.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_DETAIL_CURSOR_HPP_DEFINED
#define GP_TREE_DETAIL_CURSOR_HPP_DEFINED

#include <boost/iterator/iterator_facade.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <type_traits>

namespace gp {
namespace detail {

template< typename Node >
struct node_value_getter : public boost::mpl::eval_if<
    std::is_const< Node > ,
    std::add_const< typename Node::value_type > ,
    boost::mpl::identity< typename Node::value_type >
  >
{
};

template< typename Node >
struct node_base_getter : public boost::mpl::eval_if<
    std::is_const< Node > ,
    std::add_const< typename Node::node_base_type > ,
    boost::mpl::identity< typename Node::node_base_type >
    >
{
};

template< typename Node >
class node_cursor : public boost::iterator_facade<
    node_cursor< Node > ,                              // Derived-Iterator
    typename node_value_getter< Node >::type ,         // Value
    boost::random_access_traversal_tag ,               // Category
    boost::use_default ,                               // Reference
    boost::use_default 
    >
{
    
    friend class boost::iterator_core_access;
    template<typename U> friend class node_cursor;
    
    typedef Node node_type;
    typedef node_type* node_pointer;
    typedef typename std::remove_const< Node >::type real_node_type;    
    
    typedef typename node_base_getter< Node >::type node_base;
    typedef node_base* node_base_pointer;
    typedef typename std::remove_const< node_base >::type real_node_base;
    typedef real_node_base* real_node_base_pointer;
    

    
    typedef boost::iterator_facade<
        node_cursor< Node > ,                              // Derived-Iterator
        typename node_value_getter< Node >::type ,         // Value
        boost::random_access_traversal_tag ,               // Category
        typename node_value_getter< Node >::type& ,        // Reference
        ptrdiff_t
    > base_type;
    
    template< typename OtherNode >
    struct other_node_enabler : public std::enable_if< std::is_convertible< typename OtherNode::value_type* , typename base_type::value_type* >::value >
    {
    };
    
public:
    
    // requirements from container concept via cursor concept
    typedef typename base_type::reference const_reference;
    typedef size_t size_type;    
    
    // requirements from cursor concept
    typedef node_cursor< real_node_type > cursor;
    typedef node_cursor< real_node_type const > const_cursor;

    // typedef ascending_random_access_cursor_tag type;
    
    node_cursor( node_base_pointer node = nullptr , size_type pos = 0 )
    : m_node( node ) , m_pos( pos ) { }

    template< typename OtherNode , typename Enabler = typename other_node_enabler< OtherNode >::type >
    node_cursor( node_cursor< OtherNode > const& other )
    : m_node( other.m_node ) , m_pos( other.m_pos ) {}
    
    // emtpy()
    // size()
    // swap()
    // operator==
    // operator!=
    
    
     

    // a.begin() // cursor
    // a.end() // cursor
    // a.cbegin() const_cursor
    // a.cend() const_cursor
    // a.parity() size_type   (std::distance(b.begin(), a) if b is a's parent.)
    // a.parent() const_cursor / cursor
    
    size_type size( void ) const noexcept
    {
        return m_node->size();
    }
    
    size_type max_size( void ) const noexcept
    {
        return m_node->max_size();
    }
    
    real_node_base_pointer parent_node( void ) noexcept
    {
        return m_node;
    }
    
    const real_node_base_pointer parent_node( void ) const noexcept
    {
        return m_node;
    }
    
    real_node_base_pointer node( void ) noexcept
    {
        return m_node->children( m_pos );
    }
    
    const real_node_base_pointer node( void ) const noexcept
    {
        return m_node->children( m_pos );
    }
    
private:
    
    void increment( void )
    {
        ++m_pos;
    }
    
    void decrement( void )
    {
        --m_pos;
    }
    
    void advance( typename base_type::difference_type n )
    {
        m_pos += n;
    }
    
    typename base_type::difference_type distance_to( node_cursor const& other ) const
    {
        // TODO: implement
    }

    bool equal( node_cursor const& other) const
    {
        return ( other.m_node == m_node ) && ( other.m_pos == m_pos );
    }

    typename base_type::reference dereference() const
    {
        return **static_cast< node_pointer >( m_node->children()[m_pos] );
    }
    
    
    node_base_pointer m_node;
    size_type m_pos;
};


} // namespace detail
} // namespace gp


#endif // GP_TREE_DETAIL_CURSOR_HPP_DEFINED
