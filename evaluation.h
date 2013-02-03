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
        : 
        terminal_symbols( { '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'x' , 'y' , 'z' } ) ,
//        terminal_symbols( { 'x' , 'y' , 'z' } ) ,
//        terminal_symbols( { '1' , '3' , 'x' , 'y' , 'z' } ) ,
        unary_symbols( { 'e' } ) ,
//        unary_symbols( { 'e' , 's' , 'c' } ) ,
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
        case '1' : return 1.0;
        case '2' : return 2.0;
        case '3' : return 3.0;
        case '4' : return 4.0;
        case '5' : return 5.0;
        case '6' : return 6.0;
        case '7' : return 7.0;
        case '8' : return 8.0;
        case '9' : return 9.0;
        case 'x' : return c.x;
        case 'y' : return c.y;
        case 'z' : return c.z;
        case 'e' : return exp( eval( n->children[0] , c ) );
        case 's' : return sin( eval( n->children[0] , c ) );
        case 'c' : return cos( eval( n->children[0] , c ) );
//        case 'n' : return -( eval( n->children[0] , c ) );
        case '+' : return eval( n->children[0] , c ) + eval( n->children[1] , c );
        case '-' : return eval( n->children[0] , c ) - eval( n->children[1] , c );
        case '*' : return eval( n->children[0] , c ) * eval( n->children[1] , c );
        case '/' : return eval( n->children[0] , c ) / eval( n->children[1] , c );
        }
    }
};


#endif // EVALUATION_H_INCLUDED
