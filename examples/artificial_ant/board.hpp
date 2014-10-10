/*
 * gpcxx/examples/artificial_ant/board.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_ARTIFICIAL_ANT_BOARD_HPP_INCLUDED
#define GPCXX_EXAMPLES_ARTIFICIAL_ANT_BOARD_HPP_INCLUDED

#include <cstddef>
#include <iostream>

namespace ant_example {

enum direction
{
    north = 0, east = 1, south = 2, west = 3
};

char const * const direction_to_str( direction dir )
{
    char const * const lut[] = { "N", "E", "S", "W" };
    return lut[dir];
}


using position_1d = int;

struct position_2d
{
    int x;
    int y;
};

std::ostream & operator << ( std::ostream & os , position_2d const & pos)
{
    return os << "x" << pos.x << "y" << pos.y;
}

std::ostream & operator << ( std::ostream & os , direction const & dir)
{
    return os << direction_to_str(dir);
}

class board
{
public:
    
    board(size_t size_x, size_t size_y)
    :m_size_x(size_x), m_size_y(size_y)
    {
    }
    
    position_1d pos_2d_to_1d( position_2d pos2d ) const
    {
        return pos2d.y * m_size_x + pos2d.x;
    }
    
    position_2d pos_1d_to_2d( position_1d pos1d ) const
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
                pos2d.y -= 1;
                break;
            case east:
                pos2d.x += 1;
                break;
            case south:
                pos2d.y += 1;
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

    size_t get_size_x() const
    {
        return m_size_x;
    }
    
    size_t get_size_y() const
    {
        return m_size_y;
    }
private:
    
    size_t const m_size_x;
    size_t const m_size_y;
};

} // namespace ant_example

#endif // GPCXX_EXAMPLES_ARTIFICIAL_ANT_BOARD_HPP_INCLUDED
