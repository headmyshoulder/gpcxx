/*
 * gpcxx/tree/detail/tree_base.hpp
 * Date: 2015-02-09
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED
#define GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED

#include <gpcxx/tree/detail/basic_node.hpp>
#include <gpcxx/tree/cursor_traits.hpp>

#include <type_traits>

namespace gpcxx {

template< typename T , size_t MaxArity = 2 , typename Allocator = std::allocator< T > >
class tree_base
{
private:
    
    //
    // private types:
    //
    
    using self_type = tree_base< T , MaxArity , Allocator >;

    static const size_t max_arity = MaxArity;
    
    using node_type = detail::basic_node< T , max_arity >;
    using node_pointer = node_type*;
    
    using node_base_type = typename node_type::node_base_type;
    using node_base_pointer = node_base_type*;
    
    using node_allocator_type = typename Allocator::template rebind< node_type >::other;
    
    template< typename OtherCursor >
    using same_value_type = std::is_convertible< typename cursor_value< OtherCursor >::type , T >;
    
    template< typename OtherCursor >
    using other_cursor_enabler = typename std::enable_if< is_cursor< OtherCursor >::value && same_value_type< OtherCursor >::value >::value;

//     template< typename OtherCursor >
//     struct same_value_type
//     {
//         typedef typename std::is_convertible< typename cursor_value< OtherCursor >::type , T >::type type;
//     };
    
//     template< typename OtherCursor >
//     struct other_cursor_enabler :
//         std::enable_if< boost::mpl::and_< is_cursor< OtherCursor > , same_value_type< OtherCursor > >::value >
//     {
//     };
    
    
public:
    
    //
    // types:
    //
    using value_type = T;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = value_type const&;
//     using cursor = detail::basic_node_cursor< node_type >;
//     using const_cursor = detail::basic_node_cursor< node_type const >;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::allocator_traits< allocator_type >::pointer;
    using const_pointer = typename std::allocator_traits< allocator_type >::const_pointer;

    template< typename U , size_t M > struct rebind
    {
        typedef tree_base< U, M , typename Allocator::template rebind<U>::other > other;
    };

    
    
    //
    // construct:
    //
    explicit tree_base( allocator_type const& allocator = allocator_type() )
    : m_node_allocator( allocator ) , m_header() , m_size( 0 )
    {
    }
    
    template< typename InputCursor , typename Enabler = typename other_cursor_enabler< InputCursor >::type >
    tree_base( InputCursor subtree , allocator_type const& allocator = allocator_type() )
    : tree_base( allocator )
    {
        // insert_below( root() , subtree );
    }
    
    tree_base( tree_base const& tree )
    : tree_base( tree.get_allocator() )
    {
//         if( !tree.empty() )
//             insert_below( root() , tree.root() );
    }
    
    tree_base( tree_base const& tree , allocator_type const& allocator )
    : tree_base( allocator )
    {
//         if( !tree.empty() )
//             insert_below( root() , tree.root() );
    }
    
    tree_base( tree_base&& tree )
    : tree_base( tree.get_allocator() )
    {
//         move_impl( std::move( tree ) );
    }
    
    tree_base( tree_base&& tree , allocator_type const& allocator )
    : tree_base( allocator )
    {
//         move_impl( std::move( tree ) );
    }

    
    
    
    //
    // destroy:
    //
    ~tree_base( void )
    {
//         clear();
    }

    
private:

    //
    // members:
    //
    node_allocator_type m_node_allocator;
    node_base_type m_header;
    size_t m_size;

};


} // namespace gpcxx


#endif // GPCXX_TREE_DETAIL_TREE_BASE_HPP_INCLUDED
