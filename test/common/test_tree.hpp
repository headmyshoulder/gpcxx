/*
 * test_tree.hpp
 * Date: 2013-02-20
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TEST_TREE_HPP_INCLUDED
#define TEST_TREE_HPP_INCLUDED

#include <gpcxx/tree/basic_tree.hpp>

#include <string>

struct basic_tree_tag { };


template< typename Tag >
struct get_tree_type;

template<>
struct get_tree_type< basic_tree_tag > { typedef gpcxx::basic_tree< std::string > type; };


template< typename TreeTag >
struct test_tree
{
    typedef typename get_tree_type< TreeTag >::type tree_type;


    test_tree( void )
        : data() , data2()
    {
        {
            auto i1 = data.insert_below( data.root() , "plus" );
            auto i2 = data.insert_below( i1 , "sin" );
            auto i3 = data.insert_below( i2 , "x" );
            auto i4 = data.insert_below( i1 , "minus" );
            auto i5 = data.insert_below( i4 , "y" );
            auto i6 = data.insert_below( i4 , "2" );
        }

        {
            auto i1 = data2.insert_below( data2.root() , "minus" );
            auto i2 = data2.insert_below( i1 , "cos" );
            auto i3 = data2.insert_below( i2 , "y" );
            auto i4 = data2.insert_below( i1 , "x" );
        }
    }

    tree_type data;
    tree_type data2;
};


#endif // TEST_TREE_HPP_INCLUDED
