/*
 * pointer_types.hpp
 * Date: 2013-02-03
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef POINTER_TYPES_HPP_INCLUDED
#define POINTER_TYPES_HPP_INCLUDED

#include <boost/type_traits/integral_constant.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>


namespace gp
{
    template< class T > struct is_pointer_type : public boost::false_type { };
    template< class T > struct is_pointer_type< T* > : public boost::true_type { };
    template< class T > struct is_pointer_type< boost::shared_ptr< T > > : public boost::true_type { };
    template< class T > struct is_pointer_type< std::shared_ptr< T > > : public boost::true_type { };

    template< class T > struct unwrap_pointer;
    template< class T > struct unwrap_pointer< T* > { typedef T type; };
    template< class T > struct unwrap_pointer< boost::shared_ptr< T > > { typedef T type; };
    template< class T > struct unwrap_pointer< std::shared_ptr< T > > { typedef T type; };
};


#endif // POINTER_TYPES_HPP_INCLUDED
