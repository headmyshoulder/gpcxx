/*
 * santa_fe_ant_trail_main.cpp
 * Date: 2014-07-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 */


#include <sstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <boost/concept_check.hpp>



/*
enum move { move , turnleft , turnright };
ant, board = operation( move , ant , board )
*/

namespace santa_fe 
{
bool const X = true;
bool const s = false;

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

char const * const direction2str[] = { "N", "E", "S", "W" };

typedef int position1d;

struct  position2d
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
    
    position1d pos2dto1d(position2d pos2d) const
    {
        return pos2d.y * m_size_x + pos2d.x;
    }
    
    position2d pos1dto2d(position1d pos1d) const
    {
        int x_pos = pos1d % m_size_x;
        int y_pos = pos1d / m_size_x;
        return {x_pos, y_pos};
    }   
    
    position1d move_pos ( position1d current_pos, direction dir ) const
    {
        position2d pos2d = pos1dto2d( current_pos ) ;   
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
        return pos2dto1d(pos2d);
    }

private:
    size_t const m_size_x;
    size_t const m_size_y;
};

class ant
{
public:
    ant(position1d position, direction direction)
    :m_position(position), m_direction(direction), m_steps_done(0)
    {
    }
    
    void turnleft()
    {
        direction const lut_directions[]  = { west, north, east, south };
        m_direction = lut_directions[ m_direction ];
        m_steps_done++;
    }
    
    void turnright()
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
    
    position1d pos() const
    {
        return m_position;
    }
    
    direction dir() const
    {
        return m_direction;
    }
    
    position1d front_pos(board const & b) const
    {
        return b.move_pos(m_position, m_direction);
    }
    
    int steps_done()
    {
        return m_steps_done;
    }
    
private:
    int         m_steps_done;
    position1d  m_position;
    direction   m_direction;
};

struct has_food
{
};




class ant_simulation
{
public:
    typedef std::unordered_map< position1d, has_food > food_tail_type;
    
    ant_simulation(food_tail_type food_tail, size_t x_size, size_t y_size, position2d startpos, direction direction, int max_steps)
    :m_food_tail{food_tail}, m_board{x_size, y_size}, m_ant{m_board.pos2dto1d(startpos), direction}, m_food_start_count(food_tail.size()), m_max_steps(max_steps)
    {
    }
    
    bool food_in_front()
    {
        position1d front_pos = m_ant.front_pos(m_board);
        auto found = m_food_tail.find(front_pos);
        return found != m_food_tail.end();
    }
    
    
    void turnleft()
    {
        m_ant.turnleft();
    }
    
    void turnright()
    {
        m_ant.turnright();
    }
    
    void move()
    {
        m_ant.move(m_board);
        auto on_food = m_food_tail.find(m_ant.pos());
        if(on_food != m_food_tail.end())
        {
            m_food_tail.erase(on_food);
        }
    }
    
    bool isfinsh()
    {
        return m_food_tail.size() == 0 || m_ant.steps_done() >= m_max_steps;
    }
    
    int food_eaten()
    {
        return m_food_start_count - m_food_tail.size();
    }
        ant             m_ant;
private:
    food_tail_type  m_food_tail;
    int const       m_food_start_count;

    board           m_board;
    int             m_max_steps;
};


std::string print_ant(ant a, board b)
{
    position1d p = a.pos();
    position2d p2d = b.pos1dto2d(p);
    
    std::ostringstream oss;
    oss << direction2str[a.dir()] << p2d.x << ":" << p2d.y;
    return oss.str();
}



int main( int argc , char *argv[] )
{
    board b(santa_fe::x_size, santa_fe::y_size);
    ant_simulation::food_tail_type santa_fe_tail;
    for(int x = 0; x < santa_fe::x_size; ++x)
        for(int y = 0; y < santa_fe::y_size; ++y)
            if(santa_fe::board[y][x] == 'X')
                santa_fe_tail[b.pos2dto1d({x, y})] = has_food();
                
    ant_simulation as{santa_fe_tail, santa_fe::x_size, santa_fe::y_size, {0, 0}, east, 400 };

    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) <<"\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) <<"\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) << "\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) << "\n";
        as.turnleft();
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) <<"\n";
    as.move();
    std::cout << as.food_in_front() << " " << as.food_eaten()  << " " << print_ant(as.m_ant, b) <<"\n";

    
    return 0;
}

