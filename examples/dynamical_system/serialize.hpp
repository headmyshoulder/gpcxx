/*
 * gpcxx/examples/dynamical_system/serialize.hpp
 * Date: 2016-02-29
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_SERIALIZE_HPP_INCLUDED
#define GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_SERIALIZE_HPP_INCLUDED

#include "tree_types.hpp"
#include "generate_data.hpp"

#include <array>
#include <string>


namespace dynsys {
    
struct deserialized_system
{
    std::array< tree_type , dim > trees;
    norm_type xnorm;
    norm_type ynorm;
};

std::string serialize_winner( std::array< tree_type , dim > const& winner , norm_type const& xnorm , norm_type const& ynorm );

deserialized_system deserialize_winner( std::string const& );
    


} // namespace dynsys


#endif // GPCXX_EXAMPLES_DYNAMICAL_SYSTEM_SERIALIZE_HPP_INCLUDED
