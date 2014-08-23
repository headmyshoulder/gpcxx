/*
 * basic_generate_strategy.hpp
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
#define GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED

#include <random>
#include <stack>
#include <cassert>
#include <array>


namespace gpcxx {

template< typename Rng , typename NodeGenerator >
class basic_generate_strategy
{
public:
    
    basic_generate_strategy( Rng &rng , NodeGenerator &generator , 
                             size_t min_height , size_t max_height )
    : m_rng( rng ) , m_generator( generator )
    , m_min_height( min_height ) , m_max_height( max_height )
    { }
    

    template< typename Tree , typename GenFunc >
    std::pair< typename Tree::cursor , size_t > insert_node_impl( Tree& tree , typename Tree::cursor current , GenFunc func ) const
    {
        auto node = ( m_generator.*func )( m_rng );
        auto cursor = tree.insert_below( current , node.first );
        return std::make_pair( cursor , node.second );
    }
    
    template< typename Tree >
    std::pair< typename Tree::cursor , size_t > insert_node( Tree &tree , typename Tree::cursor current , size_t height ) const
    {
        if( height < m_min_height )
            return insert_node_impl( tree , current , &NodeGenerator::get_non_terminal_node );
        else if ( height < m_max_height )
            return insert_node_impl( tree , current , &NodeGenerator::get_node );
        else
            return insert_node_impl( tree , current , &NodeGenerator::get_terminal );
    }
      
    template< class Tree >
    void operator()( Tree &tree ) const
    {
        typedef Tree tree_type;
        typedef typename tree_type::cursor cursor;

        // first - cursor to node, second - how many childs still have to be created
        std::stack< std::pair< cursor , size_t > > gen_stack; 

        // initialize
        auto root = insert_node_impl( tree , tree.root() , &NodeGenerator::get_non_terminal_node );
        gen_stack.push( std::make_pair( root.first , root.second ) );

        size_t height = 1;
        while( !gen_stack.empty() )
        {
            cursor current = gen_stack.top().first;
            if( current.size() == gen_stack.top().second )
            {
                gen_stack.pop();
                height--;
                continue;
            }

            ++height;

            auto n = insert_node( tree , current , height );
            if( n.second > 0 ) gen_stack.push( std::make_pair( n.first , n.second ) );
            else height--;
        }
    }
    
private:
    
    Rng &m_rng;
    NodeGenerator &m_generator;
    size_t m_min_height , m_max_height;
};


template< typename Rng , typename NodeGenerator >
basic_generate_strategy< Rng , NodeGenerator >
make_basic_generate_strategy( Rng& rng , NodeGenerator& generator , size_t min_height , size_t max_height )
{
    return basic_generate_strategy< Rng , NodeGenerator >( rng , generator , min_height , max_height );
}




} // namespace gpcxx




#endif // GPCXX_GENERATE_BASIC_GENERATE_STRATEGY_HPP_INCLUDED
