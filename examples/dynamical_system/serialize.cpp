/*
 * serialize.cpp
 * Date: 2016-02-29
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "serialize.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>
#include <functional>
#include <unordered_map>

#include <iostream>

namespace dynsys {
    
std::string serialize_winner( std::array< dynsys::tree_type , dim > const& winner , norm_type const& xnorm , norm_type const& ynorm )
{
    using namespace std;
    
    boost::property_tree::ptree pt;
    pt.put( "winner.w_1" , gpcxx::polish_string( winner[0] ) );
    pt.put( "winner.w_2" , gpcxx::polish_string( winner[1] ) );
    pt.put( "winner.w_3" , gpcxx::polish_string( winner[2] ) );

    pt.put( "norm.x_1.mean" , xnorm[0].first );
    pt.put( "norm.x_1.stdev" , xnorm[0].second );
    pt.put( "norm.x_2.mean" , xnorm[1].first );
    pt.put( "norm.x_2.stdev" , xnorm[1].second );
    pt.put( "norm.x_3.mean" , xnorm[2].first );
    pt.put( "norm.x_3.stdev" , xnorm[2].second );

    pt.put( "norm.y_1.mean" , ynorm[0].first );
    pt.put( "norm.y_1.stdev" , ynorm[0].second );
    pt.put( "norm.y_2.mean" , ynorm[1].first );
    pt.put( "norm.y_2.stdev" , ynorm[1].second );
    pt.put( "norm.y_3.mean" , ynorm[2].first );
    pt.put( "norm.y_3.stdev" , ynorm[2].second );
    
    std::ostringstream str;
    boost::property_tree::write_json( str , pt );
    return str.str();
}

deserialized_system deserialize_winner(const std::string& str )
{
    using namespace gpcxx;
    
    using mapped_func = std::pair< size_t , std::function< node_type() > > ;
    using map_type = std::unordered_map< std::string , mapped_func > ;
    // using mapper_type = std::function< mapped_func( std::string const& ) > ;

    
    auto map = map_type {
        std::make_pair( "+" , std::make_pair( 2 , []() { return node_type( plus_func {} , "+" ); } ) ) ,
        std::make_pair( "-" , std::make_pair( 2 , []() { return node_type( minus_func {} , "-" ); } ) ) ,
        std::make_pair( "*" , std::make_pair( 2 , []() { return node_type( multiplies_func {} , "*" ); } ) ) ,
        std::make_pair( "/" , std::make_pair( 2 , []() { return node_type( divides_func {} , "/" ); } ) ) ,
        std::make_pair( "c" , std::make_pair( 1 , []() { return node_type( cos_func {} , "c" ); } ) ) ,
        std::make_pair( "s" , std::make_pair( 1 , []() { return node_type( sin_func {} , "s" ); } ) ) ,
        std::make_pair( "e" , std::make_pair( 1 , []() { return node_type( exp_func {} , "e" ); } ) ) ,
        std::make_pair( "l" , std::make_pair( 1 , []() { return node_type( log_func {} , "l" ); } ) ) ,
        std::make_pair( "x" , std::make_pair( 0 , []() { return node_type( array_terminal<0> {} , "x" ); } ) ) ,
        std::make_pair( "y" , std::make_pair( 0 , []() { return node_type( array_terminal<1> {} , "y" ); } ) ) ,
        std::make_pair( "z" , std::make_pair( 0 , []() { return node_type( array_terminal<2> { } , "z" ); } ) ) 
    };
    
    auto mapper = [map]( std::string const& str ) {
        auto iter = map.find( str );
        if( iter != map.end() )
        {
            return iter->second;
        }
        else
        {
            auto value = std::stod( str );
            auto func = [str,value]() { return node_type( double_terminal< double > { value } , str ); };
            return mapped_func { std::make_pair( 0 , func ) };
        }
    };
    
    deserialized_system winner;
    boost::property_tree::ptree pt;
    std::istringstream istr { str };
    boost::property_tree::read_json( istr , pt );
    gpcxx::read_polish( pt.get< std::string >( "winner.w_1" ) , winner.trees[0] , mapper );
    gpcxx::read_polish( pt.get< std::string >( "winner.w_2" ) , winner.trees[1] , mapper );
    gpcxx::read_polish( pt.get< std::string >( "winner.w_3" ) , winner.trees[2] , mapper );
    
    winner.xnorm[0].first = pt.get< double >( "norm.x_1.mean" );
    winner.xnorm[0].second = pt.get< double >( "norm.x_1.stdev" );
    winner.xnorm[1].first = pt.get< double >( "norm.x_2.mean" );
    winner.xnorm[1].second = pt.get< double >( "norm.x_2.stdev" );
    winner.xnorm[2].first = pt.get< double >( "norm.x_3.mean" );
    winner.xnorm[2].second = pt.get< double >( "norm.x_3.stdev" );
    
    winner.ynorm[0].first = pt.get< double >( "norm.y_1.mean" );
    winner.ynorm[0].second = pt.get< double >( "norm.y_1.stdev" );
    winner.ynorm[1].first = pt.get< double >( "norm.y_2.mean" );
    winner.ynorm[1].second = pt.get< double >( "norm.y_2.stdev" );
    winner.ynorm[2].first = pt.get< double >( "norm.y_3.mean" );
    winner.ynorm[2].second = pt.get< double >( "norm.y_3.stdev" );
    
    return winner;
}

    
} // namespace dynsys

