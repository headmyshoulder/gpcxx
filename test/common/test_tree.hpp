/*
 * test_tree.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_TREE_HPP_INCLUDED
#define TEST_TREE_HPP_INCLUDED

#include "intrusive_node_generator.hpp"

#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/tree/intrusive_tree.hpp>
#include <gpcxx/tree/basic_intrusive_node.hpp>
#include <gpcxx/tree/basic_named_intrusive_node.hpp>
#include <gpcxx/util/identity.hpp>

#include <string>
#include <array>

struct basic_tree_tag { };
struct intrusive_tree_tag { };

using context_type = std::array< double , 3 >;


template< typename Tag > struct get_tree_type;

template<> struct get_tree_type< basic_tree_tag >
{
    typedef gpcxx::basic_tree< std::string > type;
};

template<> struct get_tree_type< intrusive_tree_tag >
{
    typedef gpcxx::intrusive_tree< gpcxx::basic_named_intrusive_node< double , context_type const > > type;
};



template< typename Tag > struct get_node_factory
{
    typedef gpcxx::identity type;
};

template<> struct get_node_factory< intrusive_tree_tag >
{
    typedef typename get_tree_type< intrusive_tree_tag >::type tree_type;
    typedef typename tree_type::node_type node_type;
    typedef intrusive_node_generator< node_type > type;
};


template< typename TreeTag >
struct test_tree
{
    typedef typename get_tree_type< TreeTag >::type tree_type;
    typedef typename get_node_factory< TreeTag >::type factory_type;
    
    factory_type m_factory;
    


    test_tree( void )
        : data() , data2()
    {
        {
            auto i1 = data.insert_below( data.root() , m_factory( "plus" ) );
            auto i2 = data.insert_below( i1 , m_factory( "sin" ) );
            auto i3 = data.insert_below( i2 , m_factory( "x" ) );
            auto i4 = data.insert_below( i1 , m_factory( "minus" ) );
            auto i5 = data.insert_below( i4 , m_factory( "y" ) );
            auto i6 = data.insert_below( i4 , m_factory( "2" ) );
        }

        {
            auto i1 = data2.insert_below( data2.root() , m_factory( "minus" ) );
            auto i2 = data2.insert_below( i1 , m_factory( "cos" ) );
            auto i3 = data2.insert_below( i2 , m_factory( "y" ) );
            auto i4 = data2.insert_below( i1 , m_factory( "x" ) );
        }
    }

    tree_type data;
    tree_type data2;
};


#endif // TEST_TREE_HPP_INCLUDED
