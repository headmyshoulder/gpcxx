/*
 * linked_node.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LINKED_NODE_HPP_INCLUDED
#define LINKED_NODE_HPP_INCLUDED

#include <array>
#include <cstddef>
#include <iterator>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/assert.hpp>

#ifdef GP_DEBUG

#include <iostream>
#define tab "\t"
using namespace std;

#endif 




namespace gp {

namespace detail {

    struct dereference_node
    {
        template< typename > struct result;

        template< typename F , typename T >
        struct result< F ( T*& ) > { typedef T& type; };

        template< typename F , typename T >
        struct result< F ( T const*& ) > { typedef T const& type; };

        template< typename T >
        T&  operator()( T *t ) const { return *t; }
    };

} // namespace detail







template< typename T , size_t MaxArity = 2 >
class linked_node
{
public:

    const static size_t max_arity = MaxArity;

    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type const* const_pointer;
    typedef value_type& reference;
    typedef value_type const& const_reference;

    typedef linked_node< value_type , MaxArity > node_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    typedef node_type const* const_node_pointer;
    typedef node_type const& const_node_reference;

    typedef std::array< node_pointer , max_arity > children_container;

    typedef typename boost::transform_iterator<
        detail::dereference_node ,
        typename children_container::iterator > child_iterator;
    typedef typename boost::transform_iterator<
        detail::dereference_node ,
        typename children_container::const_iterator > const_child_iterator;


    // constructors
    inline linked_node( void );
    inline linked_node( value_type const& value );
    inline linked_node( const_node_reference n );
    inline linked_node( node_type&& n );
    // linked_node( node_type&& ) = delete ;


    // destructor
    inline ~linked_node( void );


    // assign operators
    inline const_node_reference operator=( const_node_reference n );
    inline const_node_reference operator=( node_type&& );
    // const_node_reference operator=( node_type&& ) = delete;


    // queries
    inline size_t arity( void ) const { return m_arity; }
    inline size_t num_elements( void ) const { return m_num_elements; }
    inline size_t height( void ) const { return m_height; }
    inline size_t level( void ) const { return m_level; }
    inline bool empty( void ) const { return ( m_arity == 0 ); }
    inline size_t size( void ) const { return m_arity; }


    inline reference value( void ) { return m_value; }
    inline const_reference value( void ) const { return m_value; }

    inline node_reference children( size_t i ) { return *m_children[i]; }
    inline const_node_reference children( size_t i ) const { return *m_children[i]; }

    inline node_pointer children_ptr( size_t i ) { return m_children[i]; }
    inline const_node_pointer children_ptr( size_t i ) const { return m_children[i]; }

    inline node_pointer parent_ptr( void ) { return m_parent; }
    inline const_node_pointer parent_ptr( void ) const { return m_parent; }

    inline node_reference at( size_t i );
    inline const_node_reference at( size_t i ) const;

    inline node_reference operator[]( size_t i );
    inline const_node_reference operator[]( size_t i ) const;

    inline node_pointer root_ptr( void ) { return ( m_parent == nullptr ) ? this : m_parent->root_ptr() ; }
    inline const_node_pointer root_ptr( void ) const { return ( m_parent == nullptr ) ? this : m_parent->root_ptr() ; }


    // modifiers
    child_iterator emplace( value_type const & value );
    child_iterator emplace_inconsistent( value_type const & value );
    child_iterator insert( const_node_reference child );

    void swap( node_reference n ) { swap( &n ); }
    void swap( child_iterator i )  { swap( *i ); }
    void swap( node_pointer n );

    // void swap_children( size_t i1 , size_t i2 );
    // void swap_children( child_iterator i1 , child_iterator i2 );

    // void erase( size_t i );
    // void erase( child_iterator i );

    void make_consistent( void );


    // iterators
    child_iterator begin( void ) { return m_children.begin(); }
    child_iterator end( void ) { return m_children.end(); }

    const_child_iterator begin( void ) const { return m_children.begin(); }
    const_child_iterator end( void ) const { return m_children.end(); }

private:

    inline void update_height_and_num_elements( size_t m_height , ptrdiff_t diff );
    inline void update_level( size_t level );
    inline void delete_children( void );

    inline void make_consistent_impl( node_reference &node );

    static void swap_real( node_pointer n1 , node_pointer n2 );   // swaps the real nodes
    static void swap_real_impl( node_pointer n1 , node_pointer n2 );   // swaps the real nodes
    static void swap_one( node_pointer n1 , node_pointer n2 );    // reallocate one node
    static void swap_two( node_pointer n1 , node_pointer n2 );    // reallocates both nodes



    size_t m_arity;
    children_container m_children;
    node_pointer m_parent;
    value_type m_value;
    size_t m_num_elements;
    size_t m_height;
    size_t m_level;

};



// default constructor
template< typename T , size_t MaxArity >
linked_node< T , MaxArity >::linked_node( void )
    : m_value()
    , m_arity( 0 )
    , m_parent( nullptr )
    , m_num_elements( 1 )
    , m_height( 1 )
    , m_level( 0 )
{
    #ifdef GP_DEBUG
    cout << "linked_node( void ) " << this << endl;
    #endif
    std::fill( m_children.begin() , m_children.end() , nullptr );
}

// emplace constructor
template< typename T , size_t MaxArity >
linked_node< T , MaxArity >::linked_node( value_type const& value )
    : m_value( value )
    , m_arity( 0 )
    , m_parent( nullptr )
    , m_num_elements( 1 )
    , m_height( 1 )
    , m_level( 0 )
{
    #ifdef GP_DEBUG
    cout << "linked_node( value_type const& value ) " << this << endl;
    #endif
    std::fill( m_children.begin() , m_children.end() , nullptr );
}

// copy constructor
template< typename T , size_t MaxArity >
linked_node< T , MaxArity >::linked_node( const_node_reference n )
    : m_value( n.m_value )
    , m_arity( n.m_arity )
    , m_children()
    , m_parent( nullptr )
    , m_num_elements( n.m_num_elements )
    , m_height( n.m_height )
    , m_level( 0 )
{
    #ifdef GP_DEBUG
    cout << "linked_node( const_node_reference n ) " << this << " " << m_value << endl;
    #endif 
    std::fill( m_children.begin() , m_children.end() , nullptr );
    for( size_t i=0 ; i<m_arity ; ++i )
    {
        m_children[i] = ( ( n.m_children[i] != 0 ) ? new node_type( *( n.m_children[i] ) ) : nullptr );
        m_children[i]->update_level( 1 );
        m_children[i]->m_parent = this;
    }
    
}

// copy constructor
template< typename T , size_t MaxArity >
linked_node< T , MaxArity >::linked_node( node_type&& n )
    : m_value( std::move( n.m_value ) )
  , m_arity( n.m_arity )
  , m_children( std::move( n.m_children ) )
  , m_parent( n.m_parent )
  , m_num_elements( n.m_num_elements )
  , m_height( n.m_height )
  , m_level( 0 )
{
    #ifdef GP_DEBUG
    cout << "linked_node( node_type&& n )  " << this << endl;
    #endif 
    for( size_t i=0 ; i<n.m_arity ; ++i ) n.m_children[i] = nullptr;
}

// destructor
template< typename T , size_t MaxArity >
linked_node< T , MaxArity >::~linked_node( void )
{
    #ifdef GP_DEBUG
    cout << "~linked_node( void ) " << this << endl;
    #endif
    delete_children();
}

// assignment operator
template< typename T , size_t MaxArity >
const linked_node< T , MaxArity >& linked_node< T , MaxArity >::operator=( const_node_reference n )
{   
    #ifdef GP_DEBUG
    cout << "operator=( const_node_reference n ) " << this << endl;
    #endif
    delete_children();

    m_value = n.m_value;
    m_arity = n.m_arity;
    for( size_t i=0 ; i<m_arity ; ++i )
    {
        m_children[i] = ( ( n.m_children[i] != 0 ) ? new node_type( *( n.m_children[i] ) ) : nullptr );
        m_children[i]->update_level( m_level + 1 );
        m_children[i]->m_parent = this;
    }
    ptrdiff_t num_elems_diff = ptrdiff_t( n.m_num_elements ) - ptrdiff_t( m_num_elements );
    m_num_elements = n.m_num_elements;
    m_height = n.m_height;

    if( m_parent != nullptr )
    {
        m_parent->update_height_and_num_elements( m_height , num_elems_diff );
    }
    
    return *this;
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::delete_children( void )
{
    for( size_t i=0 ; i<m_arity ; ++i ) delete m_children[i];
    std::fill( m_children.begin() , m_children.end() , nullptr );
    m_arity = 0;
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::update_height_and_num_elements( size_t height , ptrdiff_t diff )
{
    if( m_height < height ) m_height = height;
    m_num_elements += diff;

    if( m_parent != nullptr )
        m_parent->update_height_and_num_elements( height + 1 , diff );
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::update_level( size_t level )
{
    if( m_level == level ) return;
    m_level = level;
    for( size_t i=0 ; i<m_arity ; ++i )
        if( m_children[i] != nullptr )
            m_children[i]->update_level( level + 1 );
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::emplace( value_type const & value ) -> child_iterator
{
    BOOST_ASSERT(( m_arity < max_arity ));

    // create new node
    node_pointer new_node = new node_type( value );
    new_node->m_parent = this;

    // update node informations of this node
    new_node->m_level = m_level + 1;
    if( m_height == 1 ) m_height = 2;
    ++m_num_elements;

    // update node informations of parent
    if( m_parent != nullptr )
        m_parent->update_height_and_num_elements( m_height + 1 , 1 );

    // add node to all children
    m_children[ m_arity ] = new_node;
    ++m_arity;

    return child_iterator( m_children.begin() + m_arity - 1 );
}


// num_of_elements and height is not set, use make_consistent to set these values
template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::emplace_inconsistent( value_type const & value ) -> child_iterator
{
    BOOST_ASSERT(( m_arity < max_arity ));

    // create new node
    node_pointer new_node = new node_type( value );
    new_node->m_parent = this;

    // update node informations of this node
    new_node->m_level = m_level + 1;

    // add node to all children
    m_children[ m_arity ] = new_node;
    ++m_arity;

    return child_iterator( m_children.begin() + m_arity - 1 );
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::insert( const linked_node< T , MaxArity > &n ) -> child_iterator
{
    BOOST_ASSERT(( m_arity < max_arity ));

    node_pointer new_node = new node_type( n );

    // update node informations of this node
    new_node->m_parent = this;
    new_node->update_level( m_level + 1 );

    // do we have to update the parent ?
    m_height = new_node->m_height + 1;
    m_num_elements = m_num_elements + new_node->m_num_elements ;

    // update node informations of parent
    if( m_parent != nullptr )
        m_parent->update_height_and_num_elements( m_height + 1 , new_node->m_num_elements );

    // add node to all children
    m_children[ m_arity ] = new_node;
    ++m_arity;

    return child_iterator( m_children.begin() + m_arity - 1 );
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::swap( linked_node< T , MaxArity > *n )
{
    if( ( m_parent == nullptr ) && ( n->m_parent == nullptr ) )
    {
        swap_real( this , n );
    }
    else if( m_parent == 0 )  //  ( n->m_parent != nullptr ) 
    {
        swap_one( this , n );
    }
    else if( n->m_parent == 0 ) // ( m_parent != nullptr )
    {
        swap_one( n , this );
    }
    else // ( m_parent != nullptr ) && ( n->m_parent != nullptr )
    {
        swap_two( this , n );
    }
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::swap_real( linked_node< T , MaxArity > *n1 , linked_node< T , MaxArity > *n2 )
{
    BOOST_ASSERT(( n1->m_parent == nullptr ));
    BOOST_ASSERT(( n2->m_parent == nullptr ));
    swap_real_impl( n1 , n2 );
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::swap_real_impl( linked_node< T , MaxArity > *n1 , linked_node< T , MaxArity > *n2 )
{
    if( n1 == n2 ) return;
    std::swap( n1->m_arity , n2->m_arity );
    std::swap( n1->m_children , n2->m_children );
    std::swap( n1->m_value , n2->m_value );
    std::swap( n1->m_num_elements , n2->m_num_elements );
    std::swap( n1->m_height , n2->m_height );
    std::swap( n1->m_level , n2->m_level );

    for( size_t i=0 ; i<n1->m_arity ; ++i ) n1->children(i).m_parent = n1;
    for( size_t i=0 ; i<n2->m_arity ; ++i ) n2->children(i).m_parent = n2;
}


template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::swap_one( linked_node< T , MaxArity > *n1 , linked_node< T , MaxArity > *n2 )
{
    BOOST_ASSERT(( n1->root_ptr() != n2->root_ptr() ));
    BOOST_ASSERT(( n1->m_parent == nullptr ));
    BOOST_ASSERT(( n2->m_parent != nullptr ));

    size_t n2_level = n2->m_level;
    swap_real_impl( n1 , n2 );

    n1->update_level( 0 );
    n2->update_level( n2_level );
    n2->m_parent->update_height_and_num_elements( n2->m_height + 1 , ptrdiff_t( n2->m_num_elements ) - ptrdiff_t( n1->m_num_elements ) );
}

template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::swap_two( linked_node< T , MaxArity > *n1 , linked_node< T , MaxArity > *n2 )
{
    BOOST_ASSERT(( n1->m_parent != nullptr ));
    BOOST_ASSERT(( n2->m_parent != nullptr ));

    if( n1 == n2 ) return;

    if( n1->m_parent == n2->m_parent )
    {
        size_t i1 = 0;
        size_t i2 = 0;
        for( size_t i=0 ; i<n1->m_parent->m_arity ; ++i )
        {
            if( n1->m_parent->m_children[i] == n1 ) i1 = i;
            if( n1->m_parent->m_children[i] == n2 ) i2 = i;
        }
        BOOST_ASSERT(( ( i1 != 0 ) || ( i2 != 0 ) ));
        n1->m_parent->m_children[i1] = n2;
        n1->m_parent->m_children[i2] = n1;
    }
    else
    {
        for( size_t i=0 ; i<n1->m_parent->m_arity ; ++i )
        {
            if( n1->m_parent->m_children[i] == n1 )
            { 
                n1->m_parent->m_children[i] = n2;
                break;
            }
        }

        for( size_t i=0 ; i<n2->m_parent->m_arity ; ++i )
        {
            if( n2->m_parent->m_children[i] == n2 )
            {
                n2->m_parent->m_children[i] = n1;
                break;
            }
        }
        std::swap( n1->m_parent , n2->m_parent );

        size_t n1_level = n1->m_level;
        n1->update_level( n2->m_level );
        n2->update_level( n1_level );
        n1->m_parent->update_height_and_num_elements( n1->m_height , ptrdiff_t( n1->m_num_elements ) - ptrdiff_t( n2->m_num_elements ) );
        n2->m_parent->update_height_and_num_elements( n2->m_height , ptrdiff_t( n2->m_num_elements ) - ptrdiff_t( n1->m_num_elements ) );
    }

}


template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::make_consistent( void )
{
    make_consistent_impl( *this );
}

// calculates and returns height and num_of_elements of node
template< typename T , size_t MaxArity >
void linked_node< T , MaxArity >::make_consistent_impl( node_reference node )
{
    node.m_num_elements = 1;
    node.m_height = 1;
    for( size_t i=0 ; i<node.arity() ; ++i )
    {
        node_reference sub_node = node.children( i );
        make_consistent_impl( sub_node );
        node.m_num_elements += sub_node.m_num_elements;
        node.m_height = std::max( node.m_height , sub_node.m_height + 1 );
    }
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::at( size_t index ) -> node_reference
{
    if( index >= m_num_elements ) throw std::out_of_range( "index larger then elements in node" );
    return (*this)[index];
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::at( size_t index ) const -> const_node_reference
{
    return const_cast< node_reference >( *this ).at( index );
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::operator[]( size_t index ) -> node_reference
{
    BOOST_ASSERT(( index < m_num_elements ));
    if( index == 0 ) return *this;
    else 
    {
        size_t start = 1;
        for( size_t j=0 ; j<m_arity ; ++j )
        {
            if( index < ( start + m_children[j]->m_num_elements ) )
                return m_children[j]->at( index - start );
            start += m_children[j]->m_num_elements;
        }
    }
}

template< typename T , size_t MaxArity >
auto linked_node< T , MaxArity >::operator[]( size_t index ) const -> const_node_reference
{
    BOOST_ASSERT(( index < m_num_elements ));
    return const_cast< node_reference >( *this )[index];
}





} // namespace gp


#endif // LINKED_NODE_HPP_INCLUDED
