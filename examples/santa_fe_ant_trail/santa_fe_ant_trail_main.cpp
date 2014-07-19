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
        //wrap at corners
        pos2d.y += ( pos2d.y + 0 ) % m_size_y;
        pos2d.x += ( pos2d.x + 0 ) % m_size_x;
        
        return pos2dto1d(pos2d);
    }

private:
    size_t m_size_x;
    size_t m_size_y;
};

class ant
{
public:
    ant(position1d position, direction direction)
    :m_position(position), m_direction(direction)
    {
    }
    

    void turnleft()
    {
        direction const lut_directions[]  = { west, north, east, south };
        m_direction = lut_directions[ m_direction ];
    }
    
    void turnright()
    {
        direction const lut_directions[] = { east, south, west, north };
        m_direction = lut_directions[ m_direction ];
    }
    
    void move(board const & b)
    {
        m_position = b.move_pos(m_position, m_direction);
    }
    
    position1d pos() const
    {
        return m_position;
    }
    
    direction dir() const
    {
        return m_direction;
    }
    

private:
    position1d  m_position;
    direction   m_direction;
};


std::string print_ant(ant a, board b)
{
    position1d p = a.pos();
    position2d p2d = b.pos1dto2d(p);
    
    std::ostringstream oss;
    oss << direction2str[a.dir()] << p2d.x << ":" << p2d.y;
    return oss.str();
}

std::unordered_map< size_t, bool > has_food_;



int main( int argc , char *argv[] )
{
    board b(10, 10);
    ant a( b.pos2dto1d(position2d{0,0}) , north );
    
   // a.move(b);
    for(int i = 0; i < 20; ++i)
    {
        std::cout << print_ant( a, b ) << "\n";
        a.move(b);
    }
    
    return 0;
}
