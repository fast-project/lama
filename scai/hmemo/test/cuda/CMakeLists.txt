###
 # @file CMakeLists.txt
 #
 # @license
 # Copyright (c) 2009-2015
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
 # @brief CMake configuration file for cuda memory tests
 # @author Thomas Brandes
 # @date 16.06.2015
###

scai_project (

    SOURCES

        CUDATest
        CUDAContextTest
 
    CUDA_SOURCES

        LAMAArrayTest
)

find_package ( Boost COMPONENTS unit_test_framework )

if ( Boost_UNIT_TEST_FRAMEWORK_FOUND )

    include_directories ( ${Boost_INCLUDE_DIR} )

    cuda_add_executable ( CUDATest ${CUDA_SOURCES} ${CXX_SOURCES} )

    target_link_libraries ( CUDATest 
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        scai_hmemo
        )

else ( Boost_UNIT_TEST_FRAMEWORK_FOUND )

    message ( STATUS "memory/test/cuda/CUDATest not built, no Boost UnitTest Framework found" )

endif ( Boost_UNIT_TEST_FRAMEWORK_FOUND )
