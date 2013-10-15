/*
 * gpcxx/performance/eval_basic_ecj_cmp/parser.hpp
 * Date: 2013-09-12
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_PARSER_HPP_INCLUDED
#define GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_PARSER_HPP_INCLUDED


#include <gpcxx/tree/basic_tree.hpp>
#include <gpcxx/io/simple.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>

#include <iostream>
#include <vector>
#include <string>


namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;


    ///////////////////////////////////////////////////////////////////////////
    //  Our AST
    ///////////////////////////////////////////////////////////////////////////
    struct binary_op;
    struct unary_op;
    struct nil {};

    struct expression_ast
    {
        typedef
            boost::variant<
                nil // can't happen!
                , unsigned int
                , boost::recursive_wrapper<expression_ast>
                , boost::recursive_wrapper<binary_op>
                , boost::recursive_wrapper<unary_op>
            >
        type;

        expression_ast( void ) : expr( nil() ) {}

        template <typename Expr>
        expression_ast( Expr const& expr ) : expr( expr ) {}

        expression_ast& operator+=( expression_ast const& rhs );
        expression_ast& operator-=( expression_ast const& rhs );
        expression_ast& operator*=( expression_ast const& rhs );
        expression_ast& operator/=( expression_ast const& rhs );

        type expr;
    };

    struct binary_op
    {
        binary_op( char op , expression_ast const& left , expression_ast const& right )
        : op(op), left(left), right(right) {}

        char op;
        expression_ast left , right;
    };

    struct unary_op
    {
        unary_op( char op , expression_ast const& subject )
        : op(op), subject(subject) {}

        char op;
        expression_ast subject;
    };

    expression_ast& expression_ast::operator+=( expression_ast const& rhs )
    {
        expr = binary_op( '+' , expr , rhs );
        return *this;
    }

    expression_ast& expression_ast::operator-=( expression_ast const& rhs )
    {
        expr = binary_op( '-' , expr , rhs );
        return *this;
    }

    expression_ast& expression_ast::operator*=( expression_ast const& rhs )
    {
        expr = binary_op( '*' , expr , rhs );
        return *this;
    }

    expression_ast& expression_ast::operator/=( expression_ast const& rhs )
    {
        expr = binary_op( '/' , expr , rhs );
        return *this;
    }

    struct unary_expr
    {
        template <typename T> struct result { typedef T type; };
        char op;
        unary_expr( char _op ) : op( _op ) { }

        expression_ast operator()(expression_ast const& expr) const
        {
            return expression_ast( unary_op( op , expr ) );
        }
    };

    boost::phoenix::function< unary_expr > sin_expr( 's' );
    boost::phoenix::function< unary_expr > cos_expr( 'c' );
    boost::phoenix::function< unary_expr > exp_expr( 'e' );
    boost::phoenix::function< unary_expr > log_expr( 'l' );


    ///////////////////////////////////////////////////////////////////////////
    //  Our calculator grammar
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct grammar : qi::grammar<Iterator, expression_ast(), ascii::space_type>
    {
        grammar() : grammar::base_type(expression)
        {
            using qi::_val;
            using qi::_1;
            using qi::uint_;
            using qi::lit;

            expression =
                term                            [_val = _1 ]
                >> *(   ('+' >> term            [_val += _1])
                    |   ('-' >> term            [_val -= _1])
                    )
                ;

            term =
                factor                          [_val = _1]
                >> *(   ('*' >> factor          [_val *= _1])
                    |   ('%' >> factor          [_val /= _1])
                    )
                ;

            factor =
                  lit( "x1" )                                    [ _val = 'x' ]
                | lit( "x2" )                                    [ _val = 'y' ]
                | lit( "x3" )                                    [ _val = 'z' ]
                | ( lit( "sin" ) >> '(' >> expression >> ')' )   [ _val = sin_expr( _1 ) ]
                | ( lit( "cos" ) >> '(' >> expression >> ')' )   [ _val = cos_expr( _1 ) ]
                | ( lit( "rlog" ) >> '(' >> expression >> ')' )  [ _val = log_expr( _1 ) ]
                | ( lit( "exp" ) >> '(' >> expression >> ')' )   [ _val = exp_expr( _1 ) ]
                | ( '(' >> expression >> ')' )                   [ _val = _1 ]
                ;
        }

        qi::rule<Iterator, expression_ast(), ascii::space_type>
        expression, term, factor;
    };



    ///////////////////////////////////////////////////////////////////////////
    //  Walk the tree
    ///////////////////////////////////////////////////////////////////////////
    struct ast_print
    {
        typedef void result_type;

        void operator()( qi::info::nil ) const {}
        void operator()( char n ) const { std::cout << n; }

        void operator()( expression_ast const& ast ) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

        void operator()(binary_op const& expr) const
        {
            std::cout << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.left.expr);
            std::cout << ", ";
            boost::apply_visitor(*this, expr.right.expr);
            std::cout << ')';
        }

        void operator()(unary_op const& expr) const
        {
            std::cout << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.subject.expr);
            std::cout << ')';
        }
    };


    template< typename Tree >
    struct tree_transformator : public boost::static_visitor< void >
    {
        typedef Tree tree_type;
        typedef typename Tree::cursor cursor;
        
        
        tree_transformator( tree_type &tree , cursor c ) : tree_( tree ) , c_( c ) { }
        
    
        void operator()( qi::info::nil ) const {}
        
        void operator()( char n ) const
        {
            tree_.insert_below( c_ , n );
        }

        void operator()( expression_ast const& ast ) const
        {
            boost::apply_visitor( *this , ast.expr );
        }

        void operator()( binary_op const& expr ) const
        {
            cursor c1 = tree_.insert_below( c_ , expr.op );
            boost::apply_visitor( tree_transformator( tree_ , c1 ) , expr.left.expr );
            boost::apply_visitor( tree_transformator( tree_ , c1 ) , expr.right.expr );
        }

        void operator()( unary_op const& expr ) const
        {
            cursor c1 = tree_.insert_below( c_ , expr.op );
            boost::apply_visitor( tree_transformator( tree_ , c1 ) , expr.subject.expr );
        }

        tree_type &tree_;
        cursor c_;
    };
    
    template< typename Trafo >
    void parse_tree( std::string str , Trafo & trafo )
    {
        using boost::spirit::ascii::space;
        
        typedef std::string::const_iterator iterator_type;
        typedef grammar< iterator_type > grammar_type;
        
        grammar_type g;

    
        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        expression_ast ast;
        ast_print printer;
        bool r = phrase_parse( iter , end , g , space , ast );
        
        if (r && iter == end)
        {
            trafo( ast );
        }
        else
        {
            std::string rest(iter, end);
            throw std::runtime_error( std::string( "Parsing of expr " ) + str + " failed! Stopped at " + rest );
            // std::cout << "stopped at: \": " << rest << "\"\n";
        }
    }

} // namespace parser



#endif // GPCXX_PERFORMANCE_EVAL_BASIC_ECJ_CMP_PARSER_HPP_INCLUDED
