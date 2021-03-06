###
 # @file relativeInstall.cmake
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
 # @brief Own install command that uses relative path names
 # @author Thomas Brandes
 # @date 01.10.2015
###

# alternative version to install ( FILES f1 f2 ... DESTINATION <dest> )
# this version keeps relative pathnames of f1 f2 ...

function ( relative_install )

   set ( options )
   set ( oneValueArgs DESTINATION )
   set ( multiValueArgs FILES )

   cmake_parse_arguments ( relative_install "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

   # message( STATUS "relative_install, FILES = ${relative_install_FILES}" )
   # message( STATUS "relative_install, DESTINATION = ${relative_install_DESTINATION}" )

   foreach   ( SOURCE_FILE ${relative_install_FILES} )

       if    ( CMAKE_VERSION VERSION_GREATER 2.8.11 )
           get_filename_component( FILE_DIR ${SOURCE_FILE} DIRECTORY )
       else  ( CMAKE_VERSION VERSION_GREATER 2.8.11 )
           get_filename_component( FILE_DIR ${SOURCE_FILE} PATH )
       endif ( CMAKE_VERSION VERSION_GREATER 2.8.11 )

       # message( STATUS "install ${SOURCE_FILE} in ${relative_install_DESTINATION}/${FILE_DIR}" )

       install( FILES ${SOURCE_FILE} DESTINATION ${relative_install_DESTINATION}/${FILE_DIR} )

   endforeach ( )

endfunction ( relative_install )
