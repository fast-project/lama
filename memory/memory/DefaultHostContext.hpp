/**
 * @file DefaultHostContext.hpp
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
 * @brief DefaultHostContext.hpp
 * @author Thomas Brandes
 * @date 10.07.2011
 */

#pragma once

// for dll_import
#include <common/config.hpp>
#include <common/Thread.hpp>

// base classes
#include <memory/HostContext.hpp>

// boost
#include <boost/weak_ptr.hpp>

namespace memory
{

/** @brief This class implements the default HOST context.
 *
 *  This class is implemented as a singleton, only one default host
 *  context is available.
 *
 *  The default host context allocates/frees data in the usual way.
 */

class COMMON_DLL_IMPORTEXPORT DefaultHostContext: public HostContext
{

public:

    virtual ~DefaultHostContext();

    HostContextType getHostType() const
    {
        return HostContext::DefaultHost;
    }

    static ContextPtr getContext( int deviceNr );

    virtual void writeAt( std::ostream& stream ) const;

    virtual void* allocate( const size_t size ) const;

    virtual void allocate( ContextData& contextData, const size_t size ) const;

    virtual void free( void* pointer, const size_t size ) const;

    virtual void free( ContextData& contextData ) const;

    virtual void memcpy( void* dst, const void* src, const size_t size ) const;

    /** This routine implements Context::memcpyAsync  */

    virtual SyncToken* memcpyAsync( void* dst, const void* src, const size_t size ) const;

    virtual bool cancpy( const ContextData& dst, const ContextData& src ) const;

    virtual void memcpy( ContextData& dst, const ContextData& src, const size_t size ) const;

    virtual SyncToken* memcpyAsync( ContextData& dst, const ContextData& src, const size_t size ) const;

private:

    DefaultHostContext();

    LAMA_LOG_DECL_STATIC_LOGGER( logger )

    mutable size_t mNumberOfAllocates; //!< variable counts allocates

    mutable size_t mNumberOfAllocatedBytes;//!< variable counts allocated bytes

    mutable common::Thread::RecursiveMutex allocate_mutex;// needed to make allocate/free thread-safe

    static bool init();

    static bool initialized;   // initialization will register getContext as creator for Context
};

}