/*
 * my_listener.h
 * Date: 2013-02-18
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef MY_LISTENER_H_INCLUDED
#define MY_LISTENER_H_INCLUDED

#include <ostream>

#include <individual.hpp>
#include <listener.hpp>

std::ostream& operator<<( std::ostream &out , const de::individual &ind )
{
    if( ind.vars()->size() > 0  )
    {
        out << "(" << (*ind.vars())[0];
        for( size_t i=1 ; i<ind.vars()->size() ; ++i ) out << ", " << (*ind.vars())[i];
        out << ")";
    }
    return out;
}

class my_listener : public de::listener
{
public:

    my_listener( std::ostream &out ) : m_out( out ) { }

    void start( void )
    {
        m_out << "Starting differential evolution!" << "\n";
    }
    
    void end( void )
    {
        m_out << "Finishing differential evolution!" << "\n";
    }
    
    void error( void )
    {
        m_out << "Error occured!" << "\n";
    }
    
    void startGeneration( size_t genCount)
    {
//        m_out << "Start generation " << genCount << "\n";
    }
    
    void endGeneration( size_t genCount , de::individual_ptr bestIndGen , de::individual_ptr bestInd )
    {
        m_out << "Finished generation " << genCount << ". ";
//        m_out << "Best indiviual in Generation : " << bestIndGen->cost() << " for " << (*bestIndGen) << ". ";
        m_out << "Best overall indiviual : " << bestInd->cost() << " for " << (*bestInd) << ".";
        m_out << "\n";
    }
    
    void startSelection( size_t genCount )
    {
//        m_out << "Start selection " << genCount << "\n";
    }
    
    void endSelection( size_t genCount )
    {
//        m_out << "End selection " << genCount << "\n";
    }
    
    void startProcessors( size_t genCount )
    {
//        m_out << "Start processors " << genCount << "\n";
    }
    
    void endProcessors( size_t genCount )
    {
//        m_out << "End processors " << genCount << "\n";
    }

private:

    std::ostream &m_out;
};


#endif // MY_LISTENER_H_INCLUDED
