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


namespace gpcxx {
namespace detail {
     
namespace qi = boost::spirit::qi;

template< typename Tree , typename Cursor , typename Mapper >
struct bracket_context
{
    Tree& m_tree;
    Cursor m_cursor;
    Mapper const& m_mapper;
    bracket_context( Tree& tree , Cursor cursor , Mapper const& mapper )
    : m_tree( tree ) , m_cursor( cursor ) , m_mapper( mapper ) {}
};
    
template< typename Iterator >
struct bracket_grammar : qi::grammar< Iterator , mini_xml() , ascii::space_type >
{
    bracket_grammer()
    : bracket_grammer::base_type( xml )
    {
        using qi::lit;
        using qi::lexeme;
//         using ascii::char_;
//         using ascii::string;
//         using namespace qi::labels;
// 
//         using phoenix::at_c;
//         using phoenix::push_back;
// 
//         text = lexeme[+(char_ - '<')        [_val += _1]];
//         node = (xml | text)                 [_val = _1];
// 
//         start_tag =
//                 '<'
//             >>  !lit('/')
//             >>  lexeme[+(char_ - '>')       [_val += _1]]
//             >>  '>'
//         ;
// 
//         end_tag =
//                 "</"
//             >>  string(_r1)
//             >>  '>'
//         ;
// 
//         xml =
//                 start_tag                   [at_c<0>(_val) = _1]
//             >>  *node                       [push_back(at_c<1>(_val), _1)]
//             >>  end_tag(at_c<0>(_val))
//         ;
    }
    
//     qi::rule< Iterator , 
// 
//     qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
//     qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
//     qi::rule<Iterator, std::string(), ascii::space_type> text;
//     qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
//     qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
};


template< typename Tree , typename Cursor , typename Mapper >
Rng read_bracket_impl( std::string const& str , Tree& tree , Cursor cursor , Mapper const& mapper , std::string const& opening , std::string const& closing )
{
    
    
    // find next token
    // create node
    // if token == closing
    // if token == opening
    
    
    
//     auto rng1 = boost::algorithm::find_first( rng , opening );
//     auto rng2 = boost::algorithm::find_first( rng , closing );
//     if( boost::empty( rng1 ) && boost::empty( rng2 ) )
//         throw gpcxx_exception( "Not a bracket format: " + std::string( boost::begin( rng ) , boost::end( rng ) ) );
//     
//     Rng final_rng;
//     if( boost::begin( rng1 ) < boost::begin( rng2 ) )
//     {
//         std::string s { boost::begin( rng ) , boost::begin( rng1 ) };
//         auto c = tree.insert_below( cursor , mapper( s ) );
//         auto search_rng = read_bracket_impl( boost::make_iterator_range( boost::end( rng2 ) , boost::end( rng ) ) , tree , c , mapper , opening , closing );
//         final_rng = search_rng;
//     }
//     else
//     {
//         std::string s { boost::begin( rng ) , boost::begin( rng2 ) };
//         tree.insert_below( cursor , mapper( s ) );
//         final_rng = boost::make_iterator_range( boost::end( rng2 ) , boost::end( rng ) );
//     }
//     
//     auto closing_rng = boost::algorithm::find_first( final_rng , closing );
//     if( boost::empty( closing_rng ) )
//         throw gpcxx_exception( "No closing bracket found in " + std::string( boost::begin( rng ) , boost::end( rng ) ) );
//     
//     return boost::make_iterator_range( boost::end( closing_rng ) , boost::end( rng ) );
}


} // namespace detail
} // namespace gpcxx


#endif // GPCXX_IO_DETAIL_READ_BRACKET_HPP_INCLUDED
