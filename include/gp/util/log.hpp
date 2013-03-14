/*
 * log.hpp
 * Date: 2013-02-22
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#include <Amboss/Log/LogEntryStream.h>
#include <Amboss/Log/LoggerCollection.h>

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



class GPLogger
{
public:

    static Amboss::Log::LoggerCollection& getInstance( void )
    {
        static std::shared_ptr< Amboss::Log::LoggerCollection > instance;
        if( ! instance )
            instance = createInstance();
        return *instance;
    }

private:

    static std::shared_ptr< Amboss::Log::LoggerCollection > createInstance( void )
    {
        using namespace Amboss::Log;

        std::shared_ptr< LoggerCollection > logger = std::make_shared< LoggerCollection >();
        logger->data().clear();

        auto filter = []( const LogEntry &e ) { return ( e.logLevel >= NOISE ); };

        logger->data().push_back( std::make_shared< OStreamLogger >( std::cout , gp::DefaultFormatter() , filter ) );

        // boost::shared_ptr< std::ostream > s = boost::make_shared< std::ofstream >( "log.dat" );
        // streams.push_back( s );
        // std::shared_ptr< OStreamLogger > ll = std::make_shared< OStreamLogger >( *s , gp::DefaultFormatter() , filter );
        // logger->data().push_back( std::shared_ptr< ILogger >( ll ) );

        return logger;
    }

    GPLogger( void );
    GPLogger( const GPLogger& );
    GPLogger& operator=( const GPLogger& );
};



} // namespace gp


#define GP_LOG_LEVEL_MODULE( level , module ) AMB_LOG_ENTRY(            \
        gp::GPLogger::getInstance() ,                  \
        gp::makeGPLogEntry( level , "" , module , __FILE__ , __LINE__ ) )

#define GP_LOG_MODULE( module ) GP_LOG_LEVEL_ID( Amboss::Log::NOISE , module )

#define GP_LOG_LEVEL( level ) AMOD_LOG_LEVEL_ID( level , "" )

#define GP_LOG GP_LOG_LEVEL( Amboss::Log::NOISE )




#endif // LOG_HPP_INCLUDED
