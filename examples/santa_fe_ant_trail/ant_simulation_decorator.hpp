/*
 * gpcxx/examples/santa_fe_ant_trail/ant_simulation_decorator.hpp
 * Date: 2014-08-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_DECORATOR_HPP_INCLUDED
#define GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_DECORATOR_HPP_INCLUDED

#include "ant_simulation.hpp"

#include <functional>
#include <sstream>

namespace gpcxx {

class ant_simulation_decorator
{
public:
    ant_simulation_decorator(ant_simulation & ant_sim, std::function< void( ant_simulation_decorator const & ) > on_every_step)
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
                    oss << direction_to_str(m_ant_sim.get_ant().dir());
                else 
                {
                    auto found = m_ant_sim.get_food_trail().find(pos_1d);
                    if(found != m_ant_sim.get_food_trail().cend())
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
    
    std::function<void(ant_simulation_decorator const &)> m_on_every_step;
    ant_simulation & m_ant_sim;
};



} // namespace gpcxx


#endif // GPCXX_EXAMPLES_SANTA_FE_ANT_TRAIL_ANT_SIMULATION_DECORATOR_HPP_INCLUDED
