#! /usr/bin/python

import commands
import os
import sys
import fnmatch

root_dir = commands.getstatusoutput('git rev-parse --show-toplevel')[1]

oldname = "gp"
newname = "gpcxx"

replace_rules = []
replace_rules.append( [ oldname + "/"  , newname + "/" ] )
replace_rules.append( [ oldname.upper() + "_" , newname.upper() + "_" ] )
replace_rules.append( [ oldname + "::" , newname + "::" ] )
replace_rules.append( [ "namespace " + oldname , "namespace " + newname ] )


headers = []
sources = []
cmakes = []
for root, dirnames, filenames in os.walk( root_dir ):
    if root[ -3 : ] == "/de" : continue
    for filename in fnmatch.filter( filenames , '*.hpp' ):
        headers.append( os.path.join( root , filename ) )
    for filename in fnmatch.filter( filenames , '*.hpp.cmake' ):
        headers.append( os.path.join( root , filename ) )
    for filename in fnmatch.filter( filenames , "*.cpp" ):
        sources.append( os.path.join( root , filename ) )
    for filename in fnmatch.filter( filenames , "CMake*" ):
        cmakes.append( os.path.join( root , filename ) )
    for filename in fnmatch.filter( filenames , "*.cmake" ):
        cmakes.append( os.path.join( root , filename ) )
        

allfiles = headers + sources + cmakes

for h in allfiles :
    print h
    f1 = open( h , "r" )
    content = f1.read()
    f1.close()
    for rule in replace_rules :
        content = content.replace( rule[0] , rule[1] )
    f2 = open( h , "w" )
    f2.write( content )


