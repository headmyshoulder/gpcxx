/*
 * eval.hpp
 * Date: 2013-02-21
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef EVAL_HPP_INCLUDED
#define EVAL_HPP_INCLUDED

#include <gp/tree/linked_node_tree.hpp>
#include <gp/tree/random_symbol_generator.hpp>

#include <boost/variant.hpp>

#include <random>
#include <vector>
#include <stdexcept>

typedef boost::variant< char , double > node_value_type;
typedef gp::linked_node< node_value_type > node_type;
typedef gp::linked_node_tree< node_value_type > tree_type;



template< class Rng >
struct generators
{
    generators( Rng &rng )
        : 
        gen0( { 'x' , 'y' , 'z' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' } , rng , 0 ) ,
        gen1( { 'e' } , rng , 1 ) ,
        gen2( { '+' , '-' , '*' , '/' } , rng , 2 )
    { }

    gp::random_symbol_generator< node_type , Rng > gen0 , gen1 , gen2;
};


struct tree_eval
{
    struct context_type
    {
        double x , y , z;
    };

    struct variant_visitor : public boost::static_visitor< double >
    {
        const node_type *m_n;
        context_type &m_c;

        variant_visitor( const node_type *n , context_type & c ) : m_n( n ) , m_c( c )  { }

        double operator()( char n ) const
        {
            switch( n )
            {
            case 'x' : return m_c.x;
            case 'y' : return m_c.y;
            case 'z' : return m_c.z;
            case '1' : return 1.0;
            case '2' : return 2.0;
            case '3' : return 3.0;
            case '4' : return 4.0;
            case '5' : return 5.0;
            case '6' : return 6.0;
            case '7' : return 7.0;
            case '8' : return 8.0;
            case '9' : return 9.0;
            case 'e' : return exp( eval( m_n->children[0] , m_c ) );
            case 's' : return sin( eval( m_n->children[0] , m_c ) );
            case 'c' : return cos( eval( m_n->children[0] , m_c ) );
//        case 'n' : return -( eval( m_n->children[0] , c ) );
            case '+' : return eval( m_n->children[0] , m_c ) + eval( m_n->children[1] , m_c );
            case '-' : return eval( m_n->children[0] , m_c ) - eval( m_n->children[1] , m_c );
            case '*' : return eval( m_n->children[0] , m_c ) * eval( m_n->children[1] , m_c );
            case '/' : return eval( m_n->children[0] , m_c ) / eval( m_n->children[1] , m_c );
            default:
                throw std::runtime_error( "Unknown char" );
            }
        }

        double operator()( double a ) const
        {
            return a;
        }
    };

    static double eval( const node_type *n , context_type &c )
    {
        return boost::apply_visitor( variant_visitor( n , c ) , n->value );
    }
};



typedef std::vector< double > vector_t;

struct fitness_function
{
    struct context_type
    {
        vector_t x1 , x2 , x3 , y;
    };

    double operator()( tree_type &t , const context_type &c ) const
    {
        double chi2 = 0.0;
        for( size_t i=0 ; i<c.x1.size() ; ++i )
        {
            tree_eval::context_type cc;
            cc.x = c.x1[i];
            cc.y = c.x2[i];
            cc.z = c.x3[i];
            double yy = tree_eval::eval( t.data() , cc );
            chi2 += ( yy - c.y[i] ) * ( yy - c.y[i] );
        }
        chi2 /= double( c.x1.size() );
        return - 1.0 / ( 1.0 + chi2 );
    }
};




#endif // EVAL_HPP_INCLUDED
