/*
 * evaluation.h
 * Date: 2013-01-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

#include "tree.h"

struct eval_base
{
    std::vector< char > terminal_symbols;
    std::vector< char > unary_symbols;
    std::vector< char > binary_symbols;

    eval_base( void )
        : terminal_symbols( { 'a' , 'b' , 'c' , 'd' , 'x' , 'y' , 'z' , 'g' , 'h' , 'i' } ) ,
//          unary_symbols( { 'e' , 's' , 't' , 'n' } ) ,
          unary_symbols( { 'n' } ) ,
          binary_symbols( { '+' , '-' , '*' , '/' } )
    { }
};

struct eval_three : public eval_base
{
    struct context_t
    {
        double x , y , z;
    };

    static double eval( const tree_node< char > *n , context_t &c )
    {
        switch( n->value )
        {
        case 'a' : return 1.0;
        case 'b' : return 2.0;
        case 'c' : return 3.0;
        case 'd' : return 7.0;
        case 'g' : return 0.33;
        case 'h' : return 1.5;
        case 'i' : return 0.0;
        case 'x' : return c.x;
        case 'y' : return c.y;
        case 'z' : return c.z;
//        case 'e' : return exp( eval( n->children[0] , c ) );
//        case 's' : return sin( eval( n->children[0] , c ) );
//        case 't' : return cos( eval( n->children[0] , c ) );
        case 'n' : return -( eval( n->children[0] , c ) );
        case '+' : return eval( n->children[0] , c ) + eval( n->children[1] , c );
        case '-' : return eval( n->children[0] , c ) - eval( n->children[1] , c );
        case '*' : return eval( n->children[0] , c ) * eval( n->children[1] , c );
        case '/' : return eval( n->children[0] , c ) / eval( n->children[1] , c );
        }
    }
};


#endif // EVALUATION_H_INCLUDED
