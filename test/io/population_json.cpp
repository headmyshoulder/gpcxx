/*
 * test/io/population_json.cpp
 * Date: 2015-06-04
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gpcxx/io/population_json.hpp>
#include "../common/test_tree.hpp"

#include <gtest/gtest.h>
#include <sstream>


#define TESTNAME population_json_tests

using namespace std;

using tree_type = typename test_tree< basic_tree_tag >::tree_type;

TEST( TESTNAME , write  )
{
    test_tree< basic_tree_tag > trees;
    std::vector< tree_type > population;
    population.push_back( trees.data );
    population.push_back( trees.data2 );
    population.push_back( trees.data3 );
    population.push_back( tree_type {} );
    std::vector< double > fitness { 0.5 , 0.6 , 0.9 };
    
    std::ostringstream str;
    str << gpcxx::population_json( population , fitness );
    
    // cout << str.str() << endl;
    
    std::string ref = R"([
  {
    "tree" : 
    {
      "value" : "plus" ,
      "children" : [
        {
          "value" : "sin" ,
          "children" : [
            {
              "value" : "x" ,
              "children" : []
            }
          ]
        } ,
        {
          "value" : "minus" ,
          "children" : [
            {
              "value" : "y" ,
              "children" : []
            } ,
            {
              "value" : "2" ,
              "children" : []
            }
          ]
        }
      ]
    } , 
    "polish" : "plus|sin|x|minus|y|2" ,
    "fitness" : 0.5 , 
    "size " : 6 , 
    "height" : 3
  } , 
  {
    "tree" : 
    {
      "value" : "minus" ,
      "children" : [
        {
          "value" : "cos" ,
          "children" : [
            {
              "value" : "y" ,
              "children" : []
            }
          ]
        } ,
        {
          "value" : "x" ,
          "children" : []
        }
      ]
    } , 
    "polish" : "minus|cos|y|x" ,
    "fitness" : 0.6 , 
    "size " : 4 , 
    "height" : 3
  } , 
  {
    "tree" : 
    {
      "value" : "plus3" ,
      "children" : [
        {
          "value" : "sin" ,
          "children" : [
            {
              "value" : "x" ,
              "children" : []
            }
          ]
        } ,
        {
          "value" : "minus" ,
          "children" : [
            {
              "value" : "y" ,
              "children" : []
            } ,
            {
              "value" : "2" ,
              "children" : []
            }
          ]
        } ,
        {
          "value" : "minus" ,
          "children" : [
            {
              "value" : "cos" ,
              "children" : [
                {
                  "value" : "y" ,
                  "children" : []
                }
              ]
            } ,
            {
              "value" : "x" ,
              "children" : []
            }
          ]
        }
      ]
    } , 
    "polish" : "plus3|sin|x|minus|y|2|minus|cos|y|x" ,
    "fitness" : 0.9 , 
    "size " : 10 , 
    "height" : 4
  }
])";
    EXPECT_EQ( ref , str.str() );
}
