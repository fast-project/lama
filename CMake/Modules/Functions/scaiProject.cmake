###
 # @file scaiProject.cmake
 #
 # @license
 # Copyright (c) 2009-2013
 # Fraunhofer Institute for Algorithms and Scientific Computing SCAI
 # for Fraunhofer-Gesellschaft
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.
 # @endlicense
 #
 # @brief CMake functions for adding source and header files to a SCAI project
 # @author Thomas Brandes
 # @date 01.10.2015
###


## Help routines to set filenames in these GLOBAL variables
##
##    CXX_SOURCES  : .cpp files
##    CXX_HEADERS  : .hpp files
##    CUDA_SOURCES : .cu files
##
## Works also in project with source subdirectories by using GLOBAL variable
##  
##    SCAI_PROJECT_START_DIR  
## 
##  so that all filenames will be relative to this directory

include ( Functions/getRelativePath )

## Need to be macros not functions, because modifications of the parent scope

# scai_extend_variable ( VAR <var> SUFFIX <suffix> PREFIX <path> FILES f1 f2 ... )
# extends variable <var> with all files where prefix and suffix are utilized

macro ( scai_extend_variable )

    set ( options )
    set ( oneValueArgs VAR SUFFIX PREFIX )
    set ( multiValueArgs FILES )

    cmake_parse_arguments ( extend_var "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # message ( STATUS "extend_var_VAR = ${extend_var_VAR}" )
    # message ( STATUS "extend_var_FILES = ${extend_var_FILES}" )
    # message ( STATUS "extend_var_SUFFIX = ${extend_var_SUFFIX}" )
    # message ( STATUS "extend_var_PREFIX = ${extend_var_PREFIX}" )

    foreach ( src_file ${extend_var_FILES} )

        set ( ${extend_var_VAR} 
              ${${extend_var_VAR}}
              "${extend_var_PREFIX}${src_file}${extend_var_SUFFIX}" 
        )

    endforeach ()

endmacro ( scai_extend_variable )

# macro that adds fileanames to the global variables: CXX_SOURCES, CXX_HEADERS, CUDA_SOURCES
#
#  scai_project( CLASSES cl1 ... SOURCES s1 s2 ... HEADERS h1 h2 ... [ADD_PARENT_SCOPE] 
#
# Adds corresponding filenames with correct prefix and suffix to the variables
#
# Important: ADD_PARENT_SCOPE must be set in source subdirectories to make variables
#            visible in the parent scope

macro    ( scai_project ) 

    # specify the keywors supported in arguments 

    set ( options ADD ADD_PARENT_SCOPE )
    set ( oneValueArgs )
    set ( multiValueArgs CLASSES HEADERS SOURCES CUDA_CLASSES CUDA_SOURCES )
 
    cmake_parse_arguments ( scai_project "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # message ( STATUS "scai_project HEADERS = ${scai_project_HEADERS}" )
    # message ( STATUS "scai_project CLASSES = ${scai_project_CLASSES}" )
    # message ( STATUS "scai_project SOURCES = ${scai_project_SOURCES}" )
    # message ( STATUS "scai_project CUDA_CLASSES = ${scai_project_CUDA_CLASSES}" )
    # message ( STATUS "scai_project CUDA_SOURCES = ${scai_project_CUDA_SOURCES}" )
    # message ( STATUS "scai_project ADD = ${scai_project_ADD}" )
    # message ( STATUS "scai_project ADD_PARENT_SCOPE = ${scai_project_ADD_PARENT_SCOPE}" )

    if ( ( scai_project_ADD_PARENT_SCOPE ) OR ( scai_project_ADD ) )

        # nothing to do at beginning, but check that project has started

        if ( DEFINED SCAI_PROJECT_START_DIR )
            # that is okay
        else ()
            message( FATAL_ERROR "scai_project with ADD but not initialized before" )
        endif ()

    else ()

        # clear variables and set SCAI_PROJECT_START_DIR
        # sets the globally used variable SCAI_PROJECT_START_DIR 
        # needed to get relative path in source sub-directories 
 
        set ( SCAI_PROJECT_START_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

        # initialize the variables for all files

        set ( CXX_SOURCES "" )
        set ( CXX_HEADERS "" )
        set ( CUDA_SOURCES "" )
 
    endif ()

    # get the relative path of the current source directory in relation to the dir where project started

    get_relative_path ( RELATIVE_PATH ${SCAI_PROJECT_START_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

    # CXX_SOURCES: cpp sources

    scai_extend_variable ( VAR CXX_SOURCES
                           PREFIX ${RELATIVE_PATH} 
                           SUFFIX .cpp
                           FILES  ${scai_project_SOURCES} ${scai_project_CLASSES} 
                         )

    # CXX_HEADERS: hpp headers

    scai_extend_variable ( VAR CXX_HEADERS
                           PREFIX ${RELATIVE_PATH} 
                           SUFFIX .hpp
                           FILES  ${scai_project_HEADERS} ${scai_project_CLASSES} ${scai_project_CUDA_CLASSES} 
                         )
 
    # CUDA_SOURCES: cu for CUDA compilation
  
    scai_extend_variable ( VAR CUDA_SOURCES
                           PREFIX ${RELATIVE_PATH} 
                           SUFFIX .cu
                           FILES  ${scai_project_CUDA_CLASSES} ${scai_project_CUDA_SOURCES}
                         )
                      
    # make GLOBAL variables visible in parent scope if required
    # if option ADD_PARENT_SCOPE is set we make values visible in parent scope
 
    if ( scai_project_ADD_PARENT_SCOPE )
        set ( CXX_SOURCES ${CXX_SOURCES} PARENT_SCOPE )
        set ( CXX_HEADERS ${CXX_HEADERS} PARENT_SCOPE )
        set ( CUDA_SOURCES ${CUDA_SOURCES} PARENT_SCOPE )
    endif ()
 
endmacro ( scai_project ) 
