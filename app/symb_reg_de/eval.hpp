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
struct random_terminal_generator
{
    std::vector< char > m_symbols;
    Rng &m_rng;
    
    random_terminal_generator( const std::vector< char > &symbols , Rng &rng )
        : m_symbols( symbols ) , m_rng( rng )
    { }

    node_type* operator()( void )
    {
        assert( !m_symbols.empty() );
        return new node_type( random_symbol() , size_t( 0 ) );
    }

    node_value_type random_symbol( void )
    {
        std::uniform_int_distribution<> dice( 0 , 1 );
        if( dice( m_rng ) == 0 )
        {
            std::uniform_int_distribution< size_t > dist( 0 , m_symbols.size() - 1 );
            node_value_type v = m_symbols[ dist( m_rng ) ];
            return v;
        }
        else
        {
            std::uniform_real_distribution< double > dist( -5.0 , 5.0 );
            node_value_type v = dist( m_rng );
            return v;
        }
    }
};


template< class Rng >
struct generators
{
    generators( Rng &rng )
        : 
        gen0( { 'x' , 'y' , 'z' } , rng ) ,
        gen1( { 'e' } , rng , 1 ) ,
        gen2( { '+' , '-' , '*' , '/' } , rng , 2 )
    { }

    random_terminal_generator< Rng > gen0;
    gp::random_symbol_generator< node_type , Rng > gen1 , gen2;
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
