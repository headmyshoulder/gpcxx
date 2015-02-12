/*
 * gpcxx/test/common/test_template.hpp
 * Date: 2013-10-30
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_TEST_COMMON_TEST_TEMPLATE_HPP_INCLUDED
#define GPCXX_TEST_COMMON_TEST_TEMPLATE_HPP_INCLUDED

#include "test_tree.hpp"
#include "generic_generator.hpp"
#include <gtest/gtest.h>

// template< class T >
// struct test_template_base : public testing::Test
// {
//     typedef typename get_tree_type< T >::type tree_type;
//     typedef typename get_node_factory< T >::type node_factory_type;
//     typedef typename tree_type::cursor cursor;
//     typedef generic_generator< T > generator_type;
// 
//     test_template_base( void )
//     : m_tree()
//     { };
// 
//     tree_type m_tree;                      // empty tree
//     node_factory_type m_factory;           // factory for the nodes
//     generator_type m_gen;                  // generator for symbols and the rng
// };
// 
// 
// template <class T>
// struct test_template : public test_template_base< T >
// {
//     typedef test_tree< T > test_tree_type;
//     
//     test_template( void )
//     : test_template_base< T >() , test_tree_type() {} ;
//     
//     test_tree_type m_test_trees;
// };


template< class T >
struct test_template : public testing::Test
{
    typedef typename get_tree_type< T >::type tree_type;
    typedef typename get_node_factory< T >::type node_factory_type;
    typedef typename tree_type::cursor cursor;
    typedef test_tree< T > test_tree_type;
    typedef generic_generator< T > generator_type;

    test_template( void )
    : m_tree() , m_test_trees() {}

    tree_type m_tree;                      // empty tree
    node_factory_type m_factory;           // factory for the nodes
    test_tree_type m_test_trees;
    generator_type m_gen;                  // generator for symbols and the rng
};




#endif // GPCXX_TEST_COMMON_TEST_TEMPLATE_HPP_INCLUDED
