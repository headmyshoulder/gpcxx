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
        : terminal_symbols( { 'a' , 'b' , 'c' , 'd' , 'x' , 'y' , 'z' } ) ,
          unary_symbols( { 'e' , 's' , 't' , 'n' } ) ,
          binary_symbols( { '+' , '-' , '*' , '/' } )
    { }
};

struct eval_three : public eval_base
{
    struct context
    {
        double x , y , z;
    };

    double eval( const tree_node< char > *n , context &c ) const
    {
        switch( n->value )
        {
        case 'a' : return 1.0;
        case 'b' : return 2.0;
        case 'c' : return 3.0;
        case 'd' : return 7.0;
        case 'x' : return c.x;
        case 'y' : return c.y;
        case 'z' : return c.z;
        case 'e' : return exp( this->eval( n->children[0] , c ) );
        case 's' : return sin( this->eval( n->children[0] , c ) );
        case 't' : return cos( this->eval( n->children[0] , c ) );
        case 'n' : return -( this->eval( n->children[0] , c ) );
        case '+' : return this->eval( n->children[0] , c ) + this->eval( n->children[1] , c );
        case '-' : return this->eval( n->children[0] , c ) - this->eval( n->children[1] , c );
        case '*' : return this->eval( n->children[0] , c ) * this->eval( n->children[1] , c );
        case '/' : return this->eval( n->children[0] , c ) / this->eval( n->children[1] , c );
        }
    }
};


#endif // EVALUATION_H_INCLUDED
