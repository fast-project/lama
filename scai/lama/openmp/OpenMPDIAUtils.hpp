/**
 * @file OpenMPDIAUtils.hpp
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
 * @brief General conversion routines for DIA sparse matrices.
 * @author Thomas Brandes
 * @date 15.07.2012
 * @since 1.0.0
 */

#pragma once

// for dll_import
#include <scai/common/config.hpp>

// internal scai libraries

#include <scai/common/SCAITypes.hpp>

namespace scai
{

namespace lama
{

/** This class provides routines to converse DIA storage data to CSR storage data and vice versa.
 */

class COMMON_DLL_IMPORTEXPORT OpenMPDIAUtils
{
public:

    /** OpenMP implementation for DIAKernelTrait::getCSRSizes */

    template<typename ValueType>
    static void getCSRSizes(
        IndexType csrSizes[],
        bool diagonalFlag,
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const ValueType diaValues[],
        const ValueType eps );

    /** OpenMP implementation for DIAKernelTrait::getCSRValues.  */

    template<typename DIAValueType,typename CSRValueType>
    static void getCSRValues(
        IndexType csrJA[],
        CSRValueType csrValues[],
        const IndexType csrIA[],
        const bool diagonalFlag,
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const DIAValueType diaValues[],
        const DIAValueType eps );

    /** Implementation for DIAKernelTrait::normalGEMV  */

    template<typename ValueType>
    static void normalGEMV(
        ValueType result[],
        const ValueType alpha,
        const ValueType x[],
        const ValueType beta,
        const ValueType y[],
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const ValueType diaValues[] );

    /** Implementation for DIAKernelTrait::normalGEVM  */

    template<typename ValueType>
    static void normalGEVM(
        ValueType result[],
        const ValueType alpha,
        const ValueType x[],
        const ValueType beta,
        const ValueType y[],
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const ValueType diaValues[] );

    /** Implementation for DIAKernelTrait::jacobi  */

    template<typename ValueType>
    static void jacobi(
        ValueType solution[],
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffset[],
        const ValueType diaValues[],
        const ValueType oldSolution[],
        const ValueType rhs[],
        const ValueType omega,
        const IndexType numRows );

    /** Implemenatation for DIAKernelTrait::Reductions::absMaxVal */

    template<typename ValueType>
    static ValueType absMaxVal(
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const ValueType diaValues[] );

private:

    /** Help routine with max 9 arguments required */

    template<typename ValueType>
    static void normalGEMV_a(
        ValueType result[],
        const std::pair<ValueType, const ValueType*> ax,  // alpha, x
        const std::pair<ValueType, const ValueType*> by,  // beta, y
        const IndexType numRows,
        const IndexType numColumns,
        const IndexType numDiagonals,
        const IndexType diaOffsets[],
        const ValueType diaValues[] );

    /** Routine that registers all methods at the kernel registry. */

    static void registerKernels( bool deleteFlag );

    /** Constructor for registration. */

    OpenMPDIAUtils();

    /** Destructor for unregistration. */

    ~OpenMPDIAUtils();

    /** Static variable for registration at static initialization. */

    static OpenMPDIAUtils guard;

    SCAI_LOG_DECL_STATIC_LOGGER( logger )
};

} /* end namespace lama */

} /* end namespace scai */
