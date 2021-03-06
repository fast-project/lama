/**
 * @file MatrixTest.cpp
 *
 * @license
 * Copyright (c) 2009-2015
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
 * @brief Contains the implementation of the class MatrixTest.
 * @author: Alexander Büchel
 * @date 01.02.2012
 * @since 1.0.0
 **/

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <scai/common/SCAITypes.hpp>

#include <scai/lama/matrix/ELLSparseMatrix.hpp>
#include <scai/lama/matrix/JDSSparseMatrix.hpp>
#include <scai/lama/matrix/DIASparseMatrix.hpp>
#include <scai/lama/matrix/CSRSparseMatrix.hpp>
#include <scai/lama/matrix/COOSparseMatrix.hpp>
#include <scai/lama/matrix/DenseMatrix.hpp>

#include <scai/lama/expression/MatrixExpressions.hpp>

#include <scai/lama/matutils/MatrixCreator.hpp>

#include <scai/common/test/TestMacros.hpp>
#include <scai/lama/test/SparseMatrixHelper.hpp>
#include <scai/lama/test/Configuration.hpp>
#include <scai/lama/test/SameMatrixHelper.hpp>
#include <scai/lama/test/TestSparseMatrices.hpp>

#include <cstring>

using namespace scai::lama;
using namespace scai::hmemo;

BOOST_AUTO_TEST_SUITE( MatrixTest )

/* --------------------------------------------------------------------- */

SCAI_LOG_DEF_LOGGER( logger, "Test.MatrixTest" )

/* --------------------------------------------------------------------- */

typedef boost::mpl::list<CSRSparseMatrix<float>, ELLSparseMatrix<float>, COOSparseMatrix<double>, JDSSparseMatrix<float>,
        DIASparseMatrix<double>, DenseMatrix<double> > MatrixTypes;

/* --------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE_TEMPLATE( TypeNameTest, MatrixType, MatrixTypes )
{
    const std::string classTypeName = MatrixType::typeName();
    MatrixType matrix;
    const Matrix* ptrMatrix = &matrix;
    const std::string objTypeName = ptrMatrix->getTypeName();
    SCAI_LOG_INFO( logger, matrix << ": class name = " << classTypeName
                   << ", object name = " << objTypeName );
    BOOST_CHECK( objTypeName == classTypeName );
}

/* --------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE_TEMPLATE( RandomTest, MatrixType, MatrixTypes )
{
    typedef typename MatrixType::MatrixValueType ValueType;
    const IndexType numRows = 150;
    const IndexType numColumns = 160;
    const double density = 0.1; // 10% density
    MatrixType matrix( numRows, numColumns );
    MatrixCreator<ValueType>::fillRandom( matrix, density );
    SCAI_LOG_INFO( logger, "Random matrix = " << matrix );
    BOOST_CHECK_EQUAL( numRows, matrix.getNumRows() );
    BOOST_CHECK_EQUAL( numColumns, matrix.getNumColumns() );
    IndexType numValues = matrix.getNumValues();
// should be very unlikely that number of values diverges much from expected number. */
    BOOST_CHECK( density * numRows * numColumns * 2.0 > numValues );
    BOOST_CHECK( density * numRows * numColumns * 0.5 < numValues );
    double sparsityRate = matrix.getSparsityRate();
    BOOST_CHECK( density * 2.0 > sparsityRate );
    BOOST_CHECK( density * 0.5 < sparsityRate );
    size_t memUsage = matrix.getMemoryUsage();
    size_t denseUsage = numRows * numColumns * sizeof( ValueType );

    if ( matrix.getMatrixKind() == Matrix::SPARSE && matrix.getLocalStorage().getFormat() != Format::DIA )
    {
        BOOST_CHECK( memUsage * 2 < denseUsage );
    }
}

/* --------------------------------------------------------------------- */

// Test filename constructor and writing to file for each matrx type
BOOST_AUTO_TEST_CASE_TEMPLATE( ReadWriteTest, MatrixType, MatrixTypes )
{
    CommunicatorPtr comm = Communicator::get(); // get default, MPI or serial
    SCAI_LOG_INFO( logger, "ReadWriteTest for MatrixType = " << typeid( MatrixType ).name() );
    std::string prefix = Configuration::getInstance().getPath();
    SCAI_LOG_INFO( logger, "prefix = " << prefix );
    SCAI_LOG_INFO( logger, "readWriteTest: check loading float matrix" );
// load an available testfile matrix.
    std::string formattedInputFile = prefix + "/bp__1600.mtx";
    MatrixType formattedMatrix( formattedInputFile );
    SCAI_LOG_INFO( logger, "formatted input matrix = " << formattedMatrix );

    if ( formattedMatrix.getMatrixKind() == Matrix::SPARSE )
    {
        // just some additional test, be careful, is too large for dense matrices
        std::string formattedInputFile = prefix + "/2D_poisson_256_formatted.frm";
        std::string xdrInputFile = prefix + "/2D_poisson_256_xdr.frm";
        MatrixType formattedPoisson( formattedInputFile );
        MatrixType xdrPoisson( formattedInputFile );
        testSameMatrix( formattedPoisson, xdrPoisson );
    }

    SCAI_LOG_INFO( logger, "readWriteTest: check writing and loading formatted matrix" );
    std::string formattedFileName = prefix + "/test_matrix_formatted.tmp.frm";
    formattedMatrix.writeToFile( formattedFileName, File::FORMATTED, File::FLOAT, File::INT, File::INT );
    MatrixType readFormattedMatrix( formattedFileName );
    testSameMatrix( formattedMatrix, readFormattedMatrix );
    SCAI_LOG_INFO( logger, "readWriteTest: check writing and loading XDR matrix" );
    std::string xdrFileName = prefix + "/test_matrix_xdr.tmp.frm";
    formattedMatrix.writeToFile( xdrFileName, File::XDR, File::DOUBLE, File::LONG, File::LONG );
    MatrixType readXDRMatrix( xdrFileName );
    testSameMatrix( readXDRMatrix, formattedMatrix );
    SCAI_LOG_INFO( logger, "readWriteTest: check writing and loading binary matrix" );
    std::string binaryFileName = prefix + "/test_matrix_bin.tmp.frm";
// Be careful: binary read must fit to the format that has been used for the write
    formattedMatrix.writeToFile( binaryFileName, File::BINARY, File::INTERNAL, File::INT, File::INT );
    MatrixType readBinaryMatrix( binaryFileName );
    testSameMatrix( formattedMatrix, readBinaryMatrix );
    SCAI_LOG_INFO( logger, "readWriteTest: check writing and loading Matrix Market matrix" );
    std::string matrixMarketFileName = prefix + "/test_matrix_mm.tmp.mtx";
    formattedMatrix.writeToFile( matrixMarketFileName, File::MATRIX_MARKET, File::DOUBLE );
    MatrixType readMarketMatrix( matrixMarketFileName );
    testSameMatrix( formattedMatrix, readMarketMatrix );
}

/* --------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE_TEMPLATE( DenseMatrixMultTest, MatrixType, MatrixTypes )
{
    typedef typename MatrixType::MatrixValueType ValueType;
// DenseMatrix = MatrixType * DenseMatrix
    MatrixType matrix1 = TestSparseMatrices::n6m4MatrixE1<ValueType>();
    DenseMatrix<ValueType> matrix2 = TestSparseMatrices::n4m3MatrixE2<ValueType>();
    DenseMatrix<ValueType> matrixP( matrix1 * matrix2 );
    DenseMatrix<ValueType> matrixR = TestSparseMatrices::n6m3MatrixERes<ValueType>();
    testSameMatrix( matrixR, matrixP );
}

/* --------------------------------------------------------------------- */
// TODO Lauri (?) schlaegt fehl.
//BOOST_AUTO_TEST_CASE_TEMPLATE( DenseMatrixAddTest, MatrixType, MatrixTypes )
//{
//
//    typedef typename MatrixType::MatrixValueType ValueType;
//
//    MatrixType matrix1 = TestSparseMatrices::n6m4MatrixE1<ValueType>();
//
//    DenseMatrix<ValueType> matrixP = matrix1 + matrix1;
//
//    DenseMatrix<ValueType> matrixR  = 2.0 * matrix1;
//
//    testSameMatrix( matrixR, matrixP );
//}

/* --------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE_END(); /* MatrixTest */
