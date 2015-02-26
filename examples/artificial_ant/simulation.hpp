/*
 * gpcxx/examples/artificial_ant/ant_simulation.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_HPP_INCLUDED
#define GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_HPP_INCLUDED

#include "board.hpp"

#include <unordered_map>
#include <ostream>
#include <sstream>



namespace ant_example {

//[ant_class    
class ant
{
public:
    
    ant( position_1d position, direction direction )
    : m_steps_done( 0 ) , m_position( position ),   m_direction( direction )
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
    
    void move( board const & b )
    {
        m_position = b.move_pos( m_position, m_direction );
        m_steps_done++;
    }
    
    position_1d pos() const
    {
        return m_position;
    }
    
    void set_pos( position_1d position )
    {
        m_position = position;
    }
    
    direction dir() const
    {
        return m_direction;
    }
    
    position_1d front_pos( board const & b ) const
    {
        return b.move_pos( m_position, m_direction );
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
//]

//[ant_wold
class ant_simulation
{
public:
    using food_trail_type = std::unordered_map< position_1d, bool >;
    
    ant_simulation( food_trail_type food_trail, size_t x_size, size_t y_size, position_2d start_pos, direction start_direction, int max_steps )
    : m_food_trail{ food_trail }, 
      m_food_start_count( food_trail.size() ), 
      m_food_eaten( 0 ),
      m_ant{ 0 , start_direction }, 
      m_board{ x_size, y_size },
      m_max_steps( max_steps )
    {
        m_ant.set_pos( m_board.pos_2d_to_1d( start_pos ) );
    }
    
    bool food_in_front() const
    {
        position_1d front_pos = m_ant.front_pos( m_board );
        auto found = m_food_trail.find( front_pos );
        return found != m_food_trail.end() && found->second;
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
        m_ant.move( m_board );
        auto on_food = m_food_trail.find( m_ant.pos() );
        if(on_food != m_food_trail.end() && on_food->second)
        {
            m_food_eaten++;
            on_food->second = false;
        }
    }
    
    bool is_finsh() const
    {
        return m_food_eaten == m_food_start_count || m_ant.steps_done() >= m_max_steps;
    }
    
    int score() const
    {
        return m_food_start_count - m_food_eaten;
    }

    int steps_done() const
    {
        return m_ant.steps_done();
    }

    position_2d ant_position() const
    {
        return m_board.pos_1d_to_2d( m_ant.pos() );
    }

    direction ant_direction() const
    {
        return m_ant.dir();
    }
    
    std::string get_board_as_str() const
    {
        std::ostringstream oss;

        for( size_t y = 0; y < m_board.get_size_y() ; ++y)
        {
            for( size_t x = 0; x < m_board.get_size_x(); ++x)
            {
                position_1d pos_1d = m_board.pos_2d_to_1d({x,y});
                
                if( m_ant.pos() == pos_1d)
                    oss << direction_to_str( m_ant.dir() );
                else 
                {
                    auto found = m_food_trail.find(pos_1d);
                    if(found != m_food_trail.cend())
                        if(found->second)
                            oss << '0';
                        else
                            oss << '*';
                    else
                        oss << ' ';
                }
            }
            oss << "\n";
        }
        return oss.str();
    }
    
    
private:
    
    food_trail_type m_food_trail;
    int const       m_food_start_count;
    int             m_food_eaten;
    ant             m_ant;
    board const     m_board;
    int const       m_max_steps;
};
//]

} // namespace ant_example

#endif // GPCXX_EXAMPLES_ARTIFICIAL_ANT_ANT_SIMULATION_HPP_INCLUDED