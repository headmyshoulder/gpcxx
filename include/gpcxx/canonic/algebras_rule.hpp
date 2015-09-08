/*
 * gpcxx/canonic/algebras_rule.hpp
 * Date: 2015-09-08
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_CANONIC_ALGEBRAS_RULE_HPP_INCLUDED
#define GPCXX_CANONIC_ALGEBRAS_RULE_HPP_INCLUDED

#include <gpcxx/canonic/algebras.hpp>


namespace gpcxx {

template< typename Algebras >
class algebras_rule
{
public:
    
    algebras_rule( Algebras const& a ) : m_algebras { &a } { }
    
protected:
    
    Algebras const* m_algebras;
};



} // namespace gpcxx


#endif // GPCXX_CANONIC_ALGEBRAS_RULE_HPP_INCLUDED
