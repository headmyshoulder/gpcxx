#! /usr/bin/python

import sys
import os
import commands

def get_version( tag_name ):
    if tag_name[0] != 'v' : raise ValueError( "tag must begin with \"v\"" )
    int_strings = tag_name[ 1 : ].split( "." )
    if len( int_strings ) == 1 :
        major = int( int_strings[0] )
        return [ major , 0 , 0 ]
    if len( int_strings ) == 2 :
        major = int( int_strings[0] )
        minor = int( int_strings[1] )
        return [ major , minor , 0 ]
    raise ValueError( "format must be vX or vX.Y" )

relative_library_dir = "include/gp"
relative_config_template = "config_version.hpp.cmake"
relative_config_header = "config_version.hpp"

if len( sys.argv ) < 2:
    print "Error: Not enough arguments"
    print ""
    print "Usage : " + sys.argv[0] + " tagname"
    print ""
    print "Exiting now!"
    
tag_name = sys.argv[1]
version = get_version( tag_name )

root_dir = commands.getstatusoutput('git rev-parse --show-toplevel')[1]

config_file_template = os.path.join( root_dir , relative_library_dir , relative_config_template )
config_file_header = os.path.join( root_dir , relative_library_dir , relative_config_header )

print "Creating tag " + tag_name 
print "Version " + str( version )
print "Repository " + root_dir
print "Config header template " + config_file_template 
print "Config header " + config_file_header

print ""
print "Creating version file"
print "Commiting version file"
print "Create tag" 

template = open( config_file_template , "r" ).read()

template = template.replace( "@GP_VERSION_MAJOR@" , str( version[0] ) )
template = template.replace( "@GP_VERSION_MINOR@" , str( version[1] ) )
template = template.replace( "@GP_VERSION_PATCH@" , str( version[2] ) )
template = template.replace( "@GP_VERSION_SHA1@" , "0" )

open( config_file_header , "w" ).write( template )
os.system( "git commit -a -m \"pre commit for tagging version" + tag_name + "\" " )
os.system( "git tag -a " + tag_name + " -m \"tagging version " + tag_name + "\" " ) 


print ""
print "Finished"
print "Do not forget to PUSH THE TAGS"
