/*
  gp/eval/basic_eval.hpp

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef GP_EVAL_BASIC_EVAL_HPP_DEFINED
#define GP_EVAL_BASIC_EVAL_HPP_DEFINED



namespace gp {

template< typename Value , typename Context , typename NodeAttribute , typename NodeAttributes >
class basic_eval
{
public:
    
    typedef Value value_type;
    typedef Context context_type;
    typedef NodeAttribute node_attribute_type;
    typedef NodeAttributes node_attributes_container;
    
    basic_eval( NodeAttributes const& attributes ) { }
    
    template< typename Tree >
    Value operator()( Tree const& tree , context_type const& context ) const
    {
    }
    
private:
    
};


template< typename Value , typename Context , typename NodeAttribute , typename NodeAttributes >
basic_eval< Value , Context , NodeAttribute , NodeAttributes >
make_basic_eval( NodeAttributes const& attributes )
{
    return basic_eval< Value , Context , NodeAttribute , NodeAttributes >( attributes );
}


} // namespace gp


#endif // GP_EVAL_BASIC_EVAL_HPP_DEFINED
