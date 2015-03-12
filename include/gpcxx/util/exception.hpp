/*
 * gpcxx/util/exception.hpp
 * Date: 2015-02-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_UTIL_EXCEPTION_HPP_INCLUDED
#define GPCXX_UTIL_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <string>


namespace gpcxx {

class gpcxx_exception : public std::exception
{
public:
    
    explicit gpcxx_exception( std::string const &msg )
    : m_msg( msg )
    { }

    ~gpcxx_exception( void ) noexcept
    { }

    const char *what( void ) const noexcept override
    {
        return m_msg.c_str();
    }

private:

    std::string m_msg;
};

class tree_exception : public gpcxx_exception
{
public:
    
    using gpcxx_exception::gpcxx_exception;
};


} // namespace gpcxx


#endif // GPCXX_UTIL_EXCEPTION_HPP_INCLUDED
