/**
 * @file WriteAccess.cpp
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
 * @brief Implementation and instantiation for template class WriteAccess.
 * @author Lauretta Schubert
 * @date 22.08.2012
 * @since 1.0.0
 */

// hpp
#include <memory/WriteAccess.hpp>

// boost
#include <boost/preprocessor.hpp>

namespace memory
{

/* --------------------------------------------------------------------------- */

LAMA_LOG_DEF_TEMPLATE_LOGGER( template<typename ValueType>, WriteAccess<ValueType>::logger, "WriteAccess" )

/* --------------------------------------------------------------------------- */

template<typename ValueType>
WriteAccess<ValueType>::WriteAccess( LAMAArray<ValueType>& view, ContextPtr context, const bool keep /* = true*/)
                : mArrayView( &view )
{
    if( view.constFlag )
    {
        COMMON_THROWEXCEPTION( "write on const array not allowed" )
    }

    LAMA_LOG_DEBUG( logger, "acquire write access for " << *mArrayView << " at " << *context << ", keep = " << keep )
    mContextDataRef = mArrayView->acquireWriteAccess( context, keep );
    mData = mArrayView->get( mContextDataRef );
    LAMA_LOG_TRACE( logger, "mData = " << mData )
}

template<typename ValueType>
WriteAccess<ValueType>::WriteAccess(
    LAMAArray<ValueType>& array,
    ContextPtr context,
    const IndexType size,
    const bool keep /* = false */)

                : mArrayView( &array )

{
    if( array.constFlag )
    {
        COMMON_THROWEXCEPTION( "write on const array not allowed" )
    }

    LAMA_LOG_DEBUG( logger,
                    "acquire write access for " << *mArrayView << " at " << *context << ", size = " << size << ", keep = " << keep )

    mContextDataRef = mArrayView->acquireWriteAccess( context, keep );

    if( !keep )
    {
        mArrayView->clear( mContextDataRef );
    }

    mArrayView->resize( mContextDataRef, size );
    mData = mArrayView->get( mContextDataRef );
    LAMA_LOG_TRACE( logger, "mData = " << mData << ", mContextDataRef = " << mContextDataRef )
}

template<typename ValueType>
WriteAccess<ValueType>::WriteAccess( LAMAArray<ValueType>& array )
                : mArrayView( &array )
{
    if( array.constFlag )
    {
        COMMON_THROWEXCEPTION( "write on const array not allowed" )
    }

    LAMA_LOG_DEBUG( logger, "acquire write access for " << *mArrayView << " at first valid context " )

    const bool keepFlag = true;

    mContextDataRef = mArrayView->acquireWriteAccess( Context::getContext( Context::Host), keepFlag );
    mData = mArrayView->get( mContextDataRef );
    LAMA_LOG_TRACE( logger, "mData = " << mData << ", mContextDataRef = " << mContextDataRef )
}

template<typename ValueType>
WriteAccess<ValueType>::~WriteAccess()
{
    LAMA_LOG_DEBUG( logger, "~WriteAccess: release" )
}

template<typename ValueType>
ValueType* WriteAccess<ValueType>::get()
{
    if( !mArrayView )
    {
        COMMON_THROWEXCEPTION( "illegal get(): access has already been released." )
    }

    return mData;
}

template<typename ValueType>
void WriteAccess<ValueType>::clear()
{
    COMMON_ASSERT( mArrayView, "WriteAccess has already been released." )
    mArrayView->clear( mContextDataRef );
    mData = NULL;
    LAMA_LOG_DEBUG( logger, "cleared " << *mArrayView )
}

template<typename ValueType>
void WriteAccess<ValueType>::resize( const IndexType newSize )
{
    COMMON_ASSERT( mArrayView, "WriteAccess has already been released." )
    // do not log before check of mArrayView
    LAMA_LOG_DEBUG( logger, "resize " << *mArrayView << " to new size " << newSize )
    mArrayView->resize( mContextDataRef, newSize );
    mData = mArrayView->get( mContextDataRef );
    LAMA_LOG_TRACE( logger, "mData = " << mData )
}

template<typename ValueType>
void WriteAccess<ValueType>::reserve( const IndexType capacity )
{
    COMMON_ASSERT( mArrayView, "WriteAccess has already been released." )
    LAMA_LOG_DEBUG( logger, "reserve " << *mArrayView << " to new capacity " << capacity )
    mArrayView->reserve( mContextDataRef, capacity ); // copy = true for old data
    mData = mArrayView->get( mContextDataRef );
    LAMA_LOG_TRACE( logger, "mData = " << mData )
}

template<typename ValueType>
IndexType WriteAccess<ValueType>::capacity() const
{
    COMMON_ASSERT( mArrayView, "WriteAccess has already been released." )
    return mArrayView->capacity( mContextDataRef );
}

template<typename ValueType>
void WriteAccess<ValueType>::release()
{
    if( mArrayView )
    {
        LAMA_LOG_DEBUG( logger, "release write access for " << *mArrayView )
        mArrayView->releaseWriteAccess( mContextDataRef );
    }

    else
    {
        LAMA_LOG_DEBUG( logger, "release write access for an already released LAMAArray" )
    }

    mArrayView = 0;
    mData = 0;
}

template<typename ValueType>
void WriteAccess<ValueType>::writeAt( std::ostream& stream ) const
{
    stream << "WriteAccess to ";

    if( mArrayView )
    {
        stream << *mArrayView;
    }

    else
    {
        stream << "already releases array view.";
    }
}

// template instantiation for the supported data types

template class COMMON_DLL_IMPORTEXPORT WriteAccess<double>;
template class COMMON_DLL_IMPORTEXPORT WriteAccess<float>;
template class COMMON_DLL_IMPORTEXPORT WriteAccess<IndexType>;

} // namespace