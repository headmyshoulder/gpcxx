/*
 * santa_fe_ant_trail_main.cpp
 * Date: 2014-07-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 */


#include <sstream>
#include <iostream>
#include <unordered_map>
#include <random>

#include <gpcxx/tree/intrusive_tree.hpp>
#include <gpcxx/tree/basic_named_intrusive_node.hpp>
#include <gpcxx/generate/uniform_symbol.hpp>

namespace santa_fe 
{
size_t const x_size = 32;
size_t const y_size = 32;

char const * const board[y_size] = {
" XXX                            ",
"   X                            ",
"   X                    .XXX..  ",
"   X                    X    X  ",
"   X                    X    X  ",
"   XXXX.XXXXX       .XX..    .  ",
"            X       .        X  ",
"            X       X        .  ",
"            X       X        X  ",
"            X       X        .  ",
"            .       X        .  ",
"            X       .        X  ",
"            X       .        .  ",
"            X       X  ...XXX.  ",
"            X       X  X        ",
"            .   .X...  .        ",
"            .   .      .        ",
"            X   .      .X...    ",
"            X   X          X    ",
"            X   X          .    ",
"            X   X          .    ",
"            X   X      ...X.    ",
"            X   .      X        ",
"            X   .               ",
" ..XX..XXXXX.   X               ",
" X              X               ",
" X              X               ",
" X     .XXXXXXX..               ",
" X     X                        ",
" .     X                        ",
" .XXXX..                        ",
"                                "
};
} // namespace santa_fe 

enum direction
{
    north = 0, east = 1, south = 2, west = 3
};

char const * const direction_to_str[] = { "N", "E", "S", "W" };

typedef int position_1d;

struct  position_2d
{
    int x;
    int y;
};


class board
{
public:             
    board(size_t size_x, size_t size_y)
    :m_size_x(size_x), m_size_y(size_y)
    {
    }
    
    position_1d pos_2d_to_1d(position_2d pos2d) const
    {
        return pos2d.y * m_size_x + pos2d.x;
    }
    
    position_2d pos_1d_to_2d(position_1d pos1d) const
    {
        int x_pos = pos1d % m_size_x;
        int y_pos = pos1d / m_size_x;
        return {x_pos, y_pos};
    }   
    
    position_1d move_pos ( position_1d current_pos, direction dir ) const
    {
        position_2d pos2d = pos_1d_to_2d( current_pos ) ;   
        switch(dir)
        {
            case north:
                pos2d.y += 1;
                break;
            case east:
                pos2d.x += 1;
                break;
            case south:
                pos2d.y -= 1;
                break;
            case west:
                pos2d.x -= 1;
                break;
        }
        //wrap at border
        pos2d.y = ( pos2d.y + m_size_y ) % m_size_y;
        pos2d.x = ( pos2d.x + m_size_x ) % m_size_x;
        return pos_2d_to_1d(pos2d);
    }

private:
    size_t const m_size_x;
    size_t const m_size_y;
};

class ant
{
public:
    ant(position_1d position, direction direction)
    :m_position(position), m_direction(direction), m_steps_done(0)
    {
    }
    
    void turn_left()
    {
        direction const lut_directions[]  = { west, north, east, south };
        m_direction = lut_directions[ m_direction ];
        m_steps_done++;
    }
    
    void turn_right()
    {
        direction const lut_directions[] = { east, south, west, north };
        m_direction = lut_directions[ m_direction ];
        m_steps_done++;
    }
    
    void move(board const & b)
    {
        m_position = b.move_pos(m_position, m_direction);
        m_steps_done++;
    }
    
    position_1d pos() const
    {
        return m_position;
    }
    
    direction dir() const
    {
        return m_direction;
    }
    
    position_1d front_pos(board const & b) const
    {
        return b.move_pos(m_position, m_direction);
    }
    
    int steps_done() const
    {
        return m_steps_done;
    }
    
private:
    int         m_steps_done;
    position_1d m_position;
    direction   m_direction;
};






class ant_simulation
{
public:
    typedef std::unordered_map< position_1d, bool > food_tail_type;
    
    ant_simulation(food_tail_type food_tail, size_t x_size, size_t y_size, position_2d start_pos, direction start_direction, int max_steps)
    :m_food_tail{food_tail}, m_board{x_size, y_size}, m_ant{m_board.pos_2d_to_1d(start_pos), start_direction}, m_food_start_count(food_tail.size()), m_max_steps(max_steps)
    {
    }
    
    bool food_in_front() const
    {
        position_1d front_pos = m_ant.front_pos(m_board);
        auto found = m_food_tail.find(front_pos);
        return found != m_food_tail.end() && found->second;
    }
    
    
    void turn_left()
    {
        m_ant.turn_left();
    }
    
    void turn_right()
    {
        m_ant.turn_right();
    }
    
    void move()
    {
        m_ant.move(m_board);
        auto on_food = m_food_tail.find(m_ant.pos());
        if(on_food != m_food_tail.end() && on_food->second)
        {
            m_food_eaten++;
            on_food->second = false;
        }
    }
    
    bool is_finsh() const
    {
        return m_food_eaten == m_food_start_count || m_ant.steps_done() >= m_max_steps;
    }
    
    int food_eaten() const 
    {
        return m_food_eaten;
    }

    int score() const
    {
        return m_food_start_count - m_food_eaten;
    }
    
    friend std::ostream & operator<<(std::ostream & os, ant_simulation const & asim)
    {
        position_1d p = asim.m_ant.pos();
        position_2d p2d = asim.m_board.pos_1d_to_2d(p);
        os << direction_to_str[asim.m_ant.dir()] << p2d.x << ":" << p2d.y;
        return os;
    }
        
private:
    food_tail_type  m_food_tail;
    int const       m_food_start_count;
    int             m_food_eaten;
    ant             m_ant;
    board           m_board;
    int             m_max_steps;
};

typedef ant_simulation value_type;
typedef std::mt19937 rng_type;
typedef ant_simulation eval_context_type;
typedef ant_simulation result_type;
typedef gpcxx::basic_named_intrusive_node< result_type , eval_context_type > node_type;
typedef gpcxx::intrusive_tree< node_type > tree_type;


struct evaluator
{
    typedef eval_context_type context_type;

    int operator()( tree_type const& t , context_type & c ) const
    {
        while( !c.is_finsh() ) 
        {
            t.root()->eval( c );
        }
        return c.score();
    }
};

struct prog2                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        node.children( 0 )->eval( ant_sim );
        node.children( 1 )->eval( ant_sim );
        return ant_sim;                       
    }                                                                                                 
};

struct if_food_ahead                                                                                           
{                                                                                                     
    template< typename Context , typename Node >                                                      
    inline typename Node::result_type operator()( Context& ant_sim , Node const& node ) const         
    {
        if(ant_sim.food_in_front())
        {
            node.children( 0 )->eval( ant_sim );
        }
        else
        {
            node.children( 1 )->eval( ant_sim );
        }
        return ant_sim;                       
    }                                                                                                 
};

struct ant_move_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.move();
        return ant_sim;
    }
};

struct ant_turn_left_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.turn_left();
        return ant_sim;
    }
};

struct ant_turn_right_task_terminal
{
    template< typename Context , typename Node >
    typename Node::result_type operator()( Context & ant_sim , Node const& node ) const
    {
        ant_sim.turn_right();
        return ant_sim;
    }
};


int main( int argc , char *argv[] )
{
    board b(santa_fe::x_size, santa_fe::y_size);
    ant_simulation::food_tail_type santa_fe_tail;
    for(int x = 0; x < santa_fe::x_size; ++x)
        for(int y = 0; y < santa_fe::y_size; ++y)
            if(santa_fe::board[y][x] == 'X')
                santa_fe_tail[b.pos_2d_to_1d({x, y})] = true;
                
    ant_simulation as{santa_fe_tail, santa_fe::x_size, santa_fe::y_size, {0, 0}, east, 400 };
    
    typedef std::mt19937 rng_type;
    
    evaluator fitness_f{};
    
    
    gpcxx::uniform_symbol< node_type > terminal_gen { std::vector< node_type >{
        node_type { ant_move_task_terminal{} ,          "m" } ,
        node_type { ant_turn_left_task_terminal{} ,     "l" } ,
        node_type { ant_turn_right_task_terminal{} ,    "r" } 
    } };

    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.turn_left();
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    as.turn_left();
    as.turn_left();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << as << "\n";
    
    return 0;
}

