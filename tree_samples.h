/*
 * tree_samples.h
 * Date: 2013-01-25
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef TREE_SAMPLES_H_INCLUDED
#define TREE_SAMPLES_H_INCLUDED

template< class Tree >
void generate_simple_tree( Tree &t )
{
    typedef typename Tree::node_type node_type;

    t.m_data = 
        new node_type(
            '+' ,
            new node_type( '-' , new node_type( 'x' ) , new node_type( '5' ) ) ,
            new node_type(
                '*' , 
                new node_type( 's' , new node_type( 'y' ) ) ,
                new node_type( 'x' ) ,
                new node_type( 'z' ) 
                )
            );


    
    // data.resize( 9 );
    // data[0].value = '+';
    // data[1].value = '-';
    // data[2].value = 'x';
    // data[3].value = '5';
    // data[4].value = '*';
    // data[5].value = 's';
    // data[6].value = 'y';
    // data[7].value = 'x';
    // data[8].value = 'z';

    // data[0].arity = 2 ; data[0].child_index[0] = 4;
    // data[1].arity = 2 ; data[1].child_index[0] = 3;
    // data[2].arity = 0 ;
    // data[3].arity = 0 ;
    // data[4].arity = 3 ; data[4].child_index[0] = 7;data[4].child_index[1] = 8;
    // data[5].arity = 1 ;
    // data[6].arity = 0;
    // data[7].arity = 0 ;
    // data[8].arity = 0 ;
}


#endif // TREE_SAMPLES_H_INCLUDED
