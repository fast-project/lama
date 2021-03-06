/**
 * @file TraceConfig.hpp
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
 * @brief Definition of class that specifies the runtime configuration for tracing.
 * @author: Thomas Brandes
 * @date 11.06.2015
 */

#pragma once

// for dll_import
#include <scai/common/config.hpp>

// base classes
#include <scai/common/NonCopyable.hpp>

// interal scai libraries
#include <scai/logging.hpp>

#include <scai/common/Thread.hpp>
#include <scai/common/shared_ptr.hpp>

// std
#include <string>
#include <map>

namespace scai
{

namespace tracing
{

class TraceData;

/** Name of environment variable used to specify trace configuration. */

#define SCAI_ENV_TRACE_CONFIG "SCAI_TRACE"

/**
 * @brief This class is used to define/set the runtime configuration for tracing.
 *
 * This class is a singleton. The only one object holds member variables that specify
 * the current settings for tracing, e.g. what kind of information is collected or
 * whether it is enabled at all.
 *
 * The singleton object will be allocated on demand after program start. A static
 * object cannot be used as MPI or VampirTrace might be uninitialized.
 *
 * The destructor of the trace configuration is used to write out the collected
 * information at runtime, e.g. the collected time information of the regions.
 */

class COMMON_DLL_IMPORTEXPORT TraceConfig: private common::NonCopyable
{
public:

    /** Type definition for thread identification used in tracing. */

    typedef common::Thread::Id ThreadId;

    ~TraceConfig();

    /**
     * @brief Get reference to the actual trace configuration.
     *
     * @return the only instance of this class.
     */

    static TraceConfig& getInstance()
    {
        return *getInstancePtr();
    }

    /**
     * Get the actual trace configuration as a shared pointer.
     */
    static common::shared_ptr<TraceConfig> getInstancePtr();

    /**
     * Query if tracing is enabled.
     */
    bool isEnabled()
    {
        return mEnabled;
    }

    bool isThreadEnabled( ThreadId threadId )
    {
        return mThreadEnabled || threadId == mMaster;
    }

    bool isVampirTraceEnabled()
    {
        return mVampirTraceEnabled;
    }

    bool isTimeTraceEnabled()
    {
        return mTimeTraceEnabled;
    }

    bool isCallTreeEnabled()
    {
        return mCallTreeEnabled;
    }

    const char* getFilePrefix() const
    {
        return getInstance().mTraceFilePrefix.c_str();
    }

    /** Get trace data for the current thread.
     *
     *  @return pointers to the TraceData for the calling thread (might be NULL)
     */

    TraceData* getTraceData();

    void traceOff();

    /** Helper class for setting global trace flag in a scope
     *  (Constructor sets global flag, destructor resets it)
     */
    class TraceScope
    {
    public:

        /** Constructor sets global trace flag and saves old state. */

        TraceScope( bool flag );

        /** Destructor resets saved state of global trace flag. */

        ~TraceScope();

    private:

        bool saveFlag; // used to save state of global trace flag
    };

    static bool globalTraceFlag;

private:

    TraceConfig();

    void setParam( const std::string& param );

    void setKey( const std::string& key, const std::string& value );

    ThreadId mMaster; //!< id of mather thread

    bool mEnabled;

    bool mTimeTraceEnabled;

    bool mCallTreeEnabled;

    bool mVampirTraceEnabled;

    void enableVampirTrace( bool flag ); // switch trace on / off

    bool mThreadEnabled; //!< true if trace should also be done for threads

    std::string mTraceFilePrefix;

    /** Each thread will have its own table for region timing.
     *  Use of shared pointer for entry in map
     */

    std::map<ThreadId, common::shared_ptr<TraceData> > mTraceDataMap;

    /** Get the trace data by the id of a thread. */

    TraceData* getTraceData( ThreadId threadId );

    /** The only one instance allocated at program start. */

    static common::shared_ptr<TraceConfig> config;

    SCAI_LOG_DECL_STATIC_LOGGER( logger )
};

} /* end namespace tracing */

} /* end namespace scai */
