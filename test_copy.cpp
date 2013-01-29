/*
 * test_copy.cpp
 * Date: 2013-01-28
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "tree.h"
#include "generate_random_tree.h"
#include "evaluation.h"

#include <iostream>
#include <random>

#define tab "\t"

using namespace std;


int main( int argc , char *argv[] )
{
    std::mt19937 rng;
    eval_three evaluator;

    random_symbol_generator< char , std::mt19937 > terminal_gen( evaluator.terminal_symbols , rng , 0 );
    random_symbol_generator< char , std::mt19937 > unary_gen( evaluator.unary_symbols , rng , 1 );
    random_symbol_generator< char , std::mt19937 > binary_gen( evaluator.binary_symbols , rng , 2 );

    size_t min_height = 1 , max_height = 3;
    tree< char > t;
    generate_random_tree( t , unary_gen , binary_gen , terminal_gen , rng , min_height , max_height );

    tree< char > t2( t );

    cout << t.m_data->num_elements << " " << t2.m_data->num_elements << " " << get_num_elements( t.m_data ) << " " << get_num_elements( t2.m_data ) << "\n";

    tree< char > t3;
    tree< char > t4( t3 );
    tree< char > t5;
    t5 = t3;
    generate_random_tree( t3 , unary_gen , binary_gen , terminal_gen , rng , min_height , max_height );
    t3 = t;

    return 0;
}
