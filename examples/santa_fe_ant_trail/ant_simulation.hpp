/*
 * gpcxx/examples/santa_fe_ant_trail/ant_simulation.hpp
 * Date: 2014-08-18
 * Author: Gerard Choinka (gerard.choinka+gpcxx@gmail.com)
 * Copyright: Gerard Choinka
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_HPP_INCLUDED
#define GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_HPP_INCLUDED

#include <unordered_map>
#include <sstream>
#include <boost/function.hpp>

#include "board.hpp"

namespace ant_example {

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
    : m_food_tail{food_tail}, 
      m_board{x_size, y_size}, 
      m_ant{m_board.pos_2d_to_1d(start_pos), start_direction}, 
      m_food_start_count(food_tail.size()), 
      m_max_steps(max_steps)
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
    
    food_tail_type const &  get_food_tail() const
    {
        return m_food_tail;
    }
    
    board get_board() const 
    {
        return m_board;
    }
    
    ant const & get_ant() const
    {
        return m_ant;
    }
        
        
private:
    food_tail_type  m_food_tail;
    int const       m_food_start_count;
    int             m_food_eaten;
    ant             m_ant;
    board           m_board;
    int             m_max_steps;
};

class ant_simulation_decorator
{
public:
    ant_simulation_decorator(ant_simulation & ant_sim, boost::function< void( ant_simulation_decorator const & ) > on_every_step)
    :m_ant_sim( ant_sim ), m_on_every_step( on_every_step )
    {
    }
    
    bool food_in_front() const
    {
        return m_ant_sim.food_in_front();
    }
    
    
    void turn_left()
    {
        m_ant_sim.turn_left();
        m_on_every_step( *this );
    }
    
    void turn_right()
    {
        m_ant_sim.turn_right();
        m_on_every_step( *this );
    }
    
    void move()
    {
        m_ant_sim.move();
        m_on_every_step( *this );
    }
    
    bool is_finsh() const
    {
        return m_ant_sim.is_finsh();
    }
    
    int food_eaten() const 
    {
        return m_ant_sim.food_eaten();
    }

    int score() const
    {
        return m_ant_sim.score();
    }
    
    ant const & get_ant() const
    {
        return m_ant_sim.get_ant();
    }
    
    friend std::ostream & operator<<(std::ostream & os, ant_simulation_decorator const & asim)
    {
        os << asim.m_ant_sim;
        return os;
    }
    
    
    
    std::string get_board_as_str() const
    {
        std::ostringstream oss;
        board b = m_ant_sim.get_board();

        for(int y = 0; y < b.get_size_y() ; ++y)
        {
            for(int x = 0; x < b.get_size_x(); ++x)
            {
                int pos_1d = b.pos_2d_to_1d({x,y});
                
                if(m_ant_sim.get_ant().pos() == pos_1d)
                    oss << direction_to_str[m_ant_sim.get_ant().dir()];
                else 
                {
                    auto found = m_ant_sim.get_food_tail().find(pos_1d);
                    if(found != m_ant_sim.get_food_tail().cend())
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
    boost::function<void(ant_simulation_decorator const &)> m_on_every_step;
    ant_simulation & m_ant_sim;
};

bool operator<(ant_simulation const & lhs, ant_simulation const & rhs)
{
    return lhs.score() < rhs.score();
}

} // namespace ant_example

#endif // GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_HPP_INCLUDED