/*
 * gpcxx/operator/any_genetic_operator.hpp
 * Date: 2015-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 * Copyright: Karsten Ahnert
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef GPCXX_OPERATOR_ANY_GENETIC_OPERATOR_HPP_INCLUDED
#define GPCXX_OPERATOR_ANY_GENETIC_OPERATOR_HPP_INCLUDED

#include <vector>
#include <memory>
#include <cassert>
#include <utility>
#include <type_traits>

#ifdef GPCXX_ANY_GENETIC_OPERATOR_DEBUG
#include <iostream>
#include <boost/type_index.hpp>
#endif


namespace gpcxx {
    


template< typename Pop , typename Fitness >    
class any_genetic_operator
{
public:
    
    using population_type = Pop;
    using fitness_type = Fitness;
    using value_type = typename Pop::value_type;
    using value_iterator = typename Pop::const_iterator;
    using value_vector_type = std::vector< value_type >;
    using selection_type = std::vector< value_iterator >;
    
    any_genetic_operator( void ) = default;
    
    ~any_genetic_operator( void ) = default;
    
    template< typename T >
    any_genetic_operator( T&& t )
    : m_data( new model< std::remove_cv_t< std::remove_reference_t< T > > >( std::forward< T >( t ) ) )
    {
        #ifdef GPCXX_ANY_GENETIC_OPERATOR_DEBUG
        namespace ti = boost::typeindex;
        std::cout << "universal ctor: " << ti::type_id_with_cvr< T >().pretty_name() << " " << ti::type_id_runtime( t ).pretty_name() << std::endl;
        #endif
    }
    
    any_genetic_operator( any_genetic_operator const& op )
    : m_data( op.m_data->clone() )
    {
        #ifdef GPCXX_ANY_GENETIC_OPERATOR_DEBUG
        std::cout << "const copy ctor" << std::endl;
        #endif
    }
    
    any_genetic_operator( any_genetic_operator& op )
    : m_data( op.m_data->clone() )
    {
        #ifdef GPCXX_ANY_GENETIC_OPERATOR_DEBUG
        std::cout << "non-const copy ctor" << std::endl;
        #endif
    }
   
    any_genetic_operator( any_genetic_operator&& op ) = default;
    
    any_genetic_operator& operator=( any_genetic_operator const& op )
    {
        #ifdef GPCXX_ANY_GENETIC_OPERATOR_DEBUG
        std::cout << "copy assignment" << std::endl;
        #endif
        return *this;
    }
    
    any_genetic_operator& operator=( any_genetic_operator&& op ) = default;

    
    
    value_vector_type operator()( population_type const& pop , fitness_type const& fitness )
    {
        assert( m_data );
        return m_data->op( pop , fitness );
    }
    
    selection_type selection( population_type const& pop , fitness_type const& fitness )
    {
        assert( m_data );
        return m_data->selection( pop , fitness );
    }
    
    value_vector_type operation( selection_type const& selection )
    {
        assert( m_data );
        return m_data->op( selection );
    }
    
    operator bool( void ) const
    {
        return static_cast< bool >( m_data );
    }
    
    
private:
    
    struct concept
    {
        virtual ~concept( void ) { }
        virtual value_vector_type op( population_type const& , fitness_type const& ) = 0;
        virtual selection_type selection( population_type const& , fitness_type const& ) = 0;
        virtual value_vector_type op( selection_type const& ) = 0;
        virtual concept* clone( void ) const = 0;
    };
    
    template< typename T >
    struct model : public concept
    {
        model( T const& t ) : m_data( t ) {}
        model( T&& t ) : m_data( std::move( t ) ) {}
        ~model( void ) = default;
        value_vector_type op( population_type const& pop , fitness_type const& fitness ) override
        {
            return m_data( pop , fitness );
        }
        selection_type selection( population_type const& pop , fitness_type const& fitness ) override
        {
            return m_data.selection( pop , fitness );
        }
        value_vector_type op( selection_type const& selection ) override
        {
            return m_data.operation( selection );
        }
        concept* clone( void ) const override
        {
            return new model( m_data );
        }
        T m_data;
    };
    
    std::unique_ptr< concept > m_data;
};

/*
class Feature
{
public:

    Feature( void ) : object_( nullptr ) { }

    template< class T >
    Feature( T t ) : object_( new FeatureModel< T >( t ) ) { }

    Feature( const Feature &e ) : object_( ( e.object_ != nullptr ) ? e.object_->copy() : nullptr ) { }

    ~Feature( void ) { }

    const Feature& operator=( const Feature &e ) {
        object_.reset( ( e.object_ != nullptr ) ? e.object_->copy() : nullptr ) ;
        return *this;
    }

    void write( std::ostream &out , size_t indent ) const {
        object_->write( out , indent );
    }

private:

    struct FeatureConcept
    {
        virtual ~FeatureConcept( void ) { };
        virtual void write( std::ostream &out , size_t indent ) const = 0;
        virtual FeatureConcept* copy( void ) const = 0;
    };

    template< class T >
    struct FeatureModel : public FeatureConcept
    {
        FeatureModel( T t ) : data_( t ) { }
        ~FeatureModel( void ) { }
        void write( std::ostream &out , size_t indent ) const {
            WriteFeature< T >::write( out , data_ , indent ); }
        FeatureConcept* copy( void ) const {
            return new FeatureModel( data_ );
        }

        T data_;
    };

    std::unique_ptr< FeatureConcept > object_;
};
*/

} // namespace gpcxx


#endif // GPCXX_OPERATOR_ANY_GENETIC_OPERATOR_HPP_INCLUDED

struct T;