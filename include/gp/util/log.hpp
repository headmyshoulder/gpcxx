/*
 * log.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#include <Amboss/Log/LogEntryStream.h>

namespace gp {



using Amboss::Log::LogLevel;


const std::string DE = "DE";
const std::string FITNESS = "Fitness";
const std::string GA = "GA";
const std::string INIT = "Init";
const std::string MAIN = "Main";

inline Amboss::Log::LogEntry makeGPLogEntry(
    Amboss::Log::LogLevel logLevel ,
    const std::string &message ,
    const std::string &module , 
    const std::string &filename = "" ,
    int lineNumber = 0 
    )
{
    Amboss::Log::LogEntry l;
    l.time = Amboss::Log::LogEntry::TimeType( boost::posix_time::second_clock::local_time() );
    l.logLevel = logLevel;
    l.message = message;
    l.filename = filename;
    l.lineNumber = lineNumber;
    l.attributes.push_back( std::make_pair( "module" , module ) );
    return l;
}


struct DefaultFormatter
{
    typedef Amboss::Log::LogEntry LogEntry;
    typedef LogEntry::AttributeSequence Attributes;

    std::string operator()( const LogEntry &e ) const
    {
        std::string s;
        s += std::string( "[" ) + logLevelName( e.logLevel ) + std::string( "]" );
        auto iter = std::find_if( e.attributes.begin() , e.attributes.end() ,
                                  []( const std::pair< std::string , std::string > &p ) { return ( p.first == "module" ); } );
        if( iter != e.attributes.end() )
        {
            s += std::string( " [" ) + iter->second + "]" ;
        }
            
        return s + std::string( " : " ) + e.message + "\n";
    }
};

} // namespace gp


#define GP_LOG_LEVEL_MODULE( level , module ) AMB_LOG_ENTRY(            \
        Amboss::Log::GlobalLogger::getInstance() ,                      \
        gp::makeGPLogEntry( level , "" , module , __FILE__ , __LINE__ ) )

#define GP_LOG_MODULE( module ) GP_LOG_LEVEL_ID( Amboss::Log::NOISE , module )

#define GP_LOG_LEVEL( level ) AMOD_LOG_LEVEL_ID( level , "" )

#define GP_LOG GP_LOG_LEVEL( Amboss::Log::NOISE )




#endif // LOG_HPP_INCLUDED
