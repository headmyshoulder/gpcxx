/*
  gp/tree/detail/node.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_TREE_DETAIL_NODE_HPP_DEFINED
#define GP_TREE_DETAIL_NODE_HPP_DEFINED

#include <array>


namespace gp {
namespace detail {

    
    
template< size_t MaxArity >
class node_base
{
public:

    static const size_t max_arity = MaxArity;
    typedef node_base< max_arity > node_base_type;
    typedef std::array< node_base_type* , max_arity > children_type;
    
    node_base( node_base_type *parent = nullptr )
    : m_parent( parent ) , m_children()
    {
        for( size_t i=0 ; i<max_arity ; ++i ) m_children[i] = nullptr;
    }
    
    children_type& children( void )
    {
        return m_children;
    }
    
    children_type const& children( void ) const
    {
        return m_children;
    }
    
    node_base_type* parent( void ) 
    {
        return m_parent;
    }
    
    node_base_type const* parent( void ) const
    {
        return m_parent;
    }

    
protected:
    
    node_base_type *m_parent;
    children_type m_children;
};




template< typename T , size_t MaxArity >
class node : public node_base< MaxArity >
{
public:
    
    typedef T value_type;
    typedef node< value_type , MaxArity > node_type;
    typedef node_base< MaxArity > node_base_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* pointer;

    reference operator*() { return m_data; } 

    const_reference operator*() const { return m_data; } 
    
    node( value_type data , node_base_type *parent = nullptr ) : node_base_type( parent ) ,  m_data( std::move( data ) ) {}
 
private:

    value_type m_data;
};





} // namespace detail
} // namespace gp


#endif // GP_TREE_DETAIL_NODE_HPP_DEFINED
