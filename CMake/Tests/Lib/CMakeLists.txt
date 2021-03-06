###
 # @file CMakeLists.txt
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
 # @brief CMakeLists.txt
 # @author Thomas Brandes
 # @date 25.04.2013
 # @since 1.0.0
###

################################################
#   CMake example for simple C++ program       #
################################################

cmake_minimum_required( VERSION 2.8 )

## PROJECT NAME and supported LANGUAGES

project ( LibTest CXX )

################################################
#   Simple C++ program                         #
################################################

# default is building shared libraries, might be changed

set (BUILD_SHARED_LIBS ON CACHE BOOL

    "Build shared libraries instead of static ones.")

if ( BUILD_SHARED_LIBS )

    set ( LIBRARY_MODE SHARED )

    add_definitions( -DDLL_LINKING )

else ()

    set ( LIBRARY_MODE STATIC )

endif ()

add_library( rectangle ${LIBRARY_MODE} Rectangle.cpp Rectangle.hpp )

add_executable( example example.cpp )

target_link_libraries( example rectangle )

