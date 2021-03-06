###
 # @file Functions.cmake
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
 # @brief CMake functions and macros
 # @author Jan Ecker
 # @date 25.04.2013
 # @since 1.0.0
###

#checks whether the given value is in the value list ( pass list as "${LIST}" (doublequotes !!!) )
function ( checkValue SINGLEVALUE VALUELIST )
    set ( BOOLVALUE FALSE )
    foreach    ( ITEM ${VALUELIST} )
        if    ( ${SINGLEVALUE} MATCHES ${ITEM} )
            set ( BOOLVALUE TRUE )
        endif ( ${SINGLEVALUE} MATCHES ${ITEM} )
    endforeach ( ITEM ${VALUELIST} )
    if    ( NOT BOOLVALUE )
        message ( FATAL_ERROR "Selected Value ${SINGLEVALUE} is no valid choice out of ${VALUELIST}" )
    endif ( NOT BOOLVALUE )
endfunction ( checkValue SINGLEVALUE VALUELIST )