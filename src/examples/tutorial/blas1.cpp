/**
 * @file blas1.cpp
 *
 * @license
 * Copyright (c) 2013
 * Fraunhofer Institute for Algorithms and Scientific Computing SCAI
 * for Fraunhofer-Gesellschaft
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @endlicense
 *
 * @brief blas1.cpp
 * @author lschubert
 * @date 17.05.2013
 * $Id$
 */

/**
 * NOTE: this example is derived from the blas1 tutorial of ViennaCL
 */

// include necessary system headers
#include <iostream>
#include <time.h>
#include <stdlib.h>

// include general lama header
#include <lama.hpp>

//include basic scalar and vector types of LAMA
#include <lama/Scalar.hpp>
#include <lama/DenseVector.hpp>

//include for using different Contexts (e.g. CUDA, default: Host)
#include <lama/Context.hpp>

//include for using the NoDistribution
#include <lama/distribution/NoDistribution.hpp>

//include the generic inner product functions of LAMA
#include <lama/expression/all.hpp>

//include the generic norm functions of LAMA
#include <lama/norm/all.hpp>

int main()
{
  srand( (unsigned int)time(NULL) );

  //Change this type definition to double if your gpu supports that
  typedef float       ScalarType;

  /////////////////////////////////////////////////
  ///////////// Scalar operations /////////////////
  /////////////////////////////////////////////////

  //
  // Define a few scalars:
  //
  lama::Scalar s1( static_cast<ScalarType>( 3.1415926 ) ); // static_cast is only needed to switch between float and double by typedef
  lama::Scalar s2( static_cast<ScalarType>( 2.71763 ) );
  lama::Scalar s3( static_cast<ScalarType>( 42.0 ) );

  // pure scalar operations only can be executed on the host
  std::cout << "Manipulating a few scalars..." << std::endl;
  std::cout << "operator +=" << std::endl;
  s1 += s2;

  std::cout << "operator *=" << std::endl;
  s1 *= s2;

  std::cout << "operator -=" << std::endl;
  s1 -= s2;;

  std::cout << "operator /=" << std::endl;
  s1 /= s2;

  std::cout << "operator +" << std::endl;
  s1 = s2 + s3;

  std::cout << "multiple operators" << std::endl;
  s1 = s2 + s3 * s2 - s3 / s1;

  //
  // Output stream is overloaded as well:
  //

  std::cout << "scalar s3: " << s3 << std::endl;


  /////////////////////////////////////////////////
  ///////////// Vector operations /////////////////
  /////////////////////////////////////////////////

  //
  // Define a few vectors and fill them with random values
  //
  ScalarType plain_vec[ 10 ];

  for ( unsigned int i = 0; i < 10; ++i )
  {
    plain_vec[ i ] = static_cast<ScalarType>( rand() ) / static_cast<ScalarType>( RAND_MAX );
  }

  lama::DenseVector<ScalarType> lama_vec1( 10, plain_vec );

  lama::LAMAArray<ScalarType> lama_array1 ( 10, plain_vec );
  lama::DenseVector<ScalarType> lama_vec2( 10, 0.0 );
  lama_vec2.setValues( lama_array1 );

  lama::DistributionPtr noDist( new lama::NoDistribution( 10 ) );
  lama::DenseVector<ScalarType> lama_vec3( lama_array1, noDist  );

  //
  // Define the vectors to be used on GPU (CUDA context on device 0) and upload them
  //
  lama::ContextPtr cudaContext = lama::ContextFactory::getContext( lama::Context::CUDA, 0 );
  lama_vec1.setContext( cudaContext );
  lama_vec2.setContext( cudaContext );
  lama_vec1.prefetch();
  lama_vec1.prefetch();

  lama_vec3.prefetch( cudaContext );

  //
  // Compute the inner product of two GPU vectors and write the result to either CPU or GPU
  //

  s1 = lama_vec1.dotProduct( lama_vec2 );

  //
  // Compute norms:
  //

  s1 = lama_vec1.l1Norm();
  s2 = lama_vec2.l2Norm();
  s3 = lama_vec3.maxNorm();

  //
  // Plane rotation of two vectors:
  // Computes (x,y) <- (alpha * x + beta * y, -beta * x + alpha * y)
  //

  float alpha = 1.1f;
  float beta = 2.3f;
  lama_vec1 = alpha * lama_vec1 + beta * lama_vec2;
  lama_vec2 = -beta * lama_vec1 + alpha * lama_vec2;

  //
  // Use lama::vector via the overloaded operators just as you would write it on paper:
  //

  //simple expression:
//  lama_vec1 = s1 * lama_vec2 / s3;

  //more complicated expression:
//  lama_vec1 = lama_vec2 / s3 + s2 * ( lama_vec1 - s2 * lama_vec2 );

  //
  // Swap the content of two vectors without a temporary vector:
  //

  lama_vec1.swap( lama_vec2 );  //swaps all entries in memory

  //
  //  That's it.
  //
  std::cout << "!!!! TUTORIAL COMPLETED SUCCESSFULLY !!!!" << std::endl;

  return EXIT_SUCCESS;
}