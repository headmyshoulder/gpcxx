/*
 * gpcxx/io/detail/read_bracket.hpp
 * Date: 2015-08-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
#define GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED

#include <gpcxx/util/exception.hpp>

// #include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
// #include <boost/spirit/include/phoenix_core.hpp>
// #include <boost/spirit/include/phoenix_operator.hpp>
// #include <boost/spirit/include/phoenix_fusion.hpp>
// #include <boost/spirit/include/phoenix_stl.hpp>
// #include <boost/fusion/include/adapt_struct.hpp>
// #include <boost/variant/recursive_variant.hpp>
// #include <boost/foreach.hpp>
// #include <string>

#include <iostream>


namespace gpcxx {
namespace detail {

    
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;


template< typename Iterator >
struct bracket_grammar : qi::grammar< Iterator , int , ascii::space_type >
{
    bracket_grammar( std::string const& opening , std::string const& closing )
    : bracket_grammar::base_type( m_tree )
    {
        m_identifier = qi::lexeme[ ( qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9") ) ];
        m_number = qi::double_;
        m_operator = qi::lexeme[ qi::char_( "+*/&%?|<>#" ) || qi::char_( '-' ) ];
        m_terminal = m_identifier | m_number | m_operator;
        
        m_node =
               m_terminal
            >> *m_subtree;
            
        m_subtree =
               qi::lit( opening )
            >> m_node
            >> qi::lit( closing );
            
        m_tree = - m_subtree;
    }
    
    qi::rule< Iterator , int , ascii::space_type > m_tree;
    qi::rule< Iterator , int , ascii::space_type > m_subtree;
    qi::rule< Iterator , int , ascii::space_type > m_node;
    qi::rule< Iterator , int , ascii::space_type > m_identifier;
    qi::rule< Iterator , int , ascii::space_type > m_number;
    qi::rule< Iterator , int , ascii::space_type > m_operator;
    qi::rule< Iterator , int , ascii::space_type > m_terminal;
};


template< typename Tree , typename Cursor , typename Mapper >
void read_bracket_impl( std::string const& str , Tree& tree , Cursor cursor , Mapper const& mapper , std::string const& opening , std::string const& closing )
{
    using grammar = bracket_grammar< std::string::const_iterator >;
    
    grammar g { opening , closing };
    // client::mini_xml ast; // Our tree
    
    using boost::spirit::ascii::space;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    int t;
    bool r = qi::phrase_parse( iter , end , g , space , t );
    
    if( ! r || ( iter != end ) )
    {
        throw gpcxx_exception( "Parsing error in bracket: " + std::string( iter , end ) );
    }
}


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
