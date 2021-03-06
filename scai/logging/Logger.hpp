/**
 * @file Logger.hpp
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
 * @brief Abstract logger class for a hierarchically organized logging system.
 * @author Thomas Brandes
 * @date 10.06.2015
 */

#pragma once

// for dll import
#include <scai/common/config.hpp>

// local library
#include <scai/logging/SourceLocation.hpp>
#include <scai/logging/Level.hpp>

// std
#include <string>
#include <vector>

namespace scai
{

/** Namespace for logging data structures */

namespace logging
{

/**************************************************************************
 *  class Logger                                                           *
 **************************************************************************/

/** Logger is a an abstract class for hierarchical organization of logging objects

 LoggerProvider is allowed to traverse the hierarchy of loggers and
 to create missing ones (friend class).

 */

class COMMON_DLL_IMPORTEXPORT Logger
{

    friend class LoggerProvider;

public:

    /** Virtual constructor needed due to virtual functions */

    virtual ~Logger();

    /** get the full name of the logger, e.g. "X.Y.Z" */

    std::string getFullName() const;

    /** Check if logging statements of level TRACE are enabled. */

    inline bool isTraceEnabled() const
    {
        return mLevel <= level::TRACE;
    }

    /** Check if logging statements of level DEBUG are enabled. */

    inline bool isDebugEnabled() const
    {
        return mLevel <= level::DEBUG;
    }

    /** Check if logging statements of level INFO are enabled. */

    inline bool isInfoEnabled() const
    {
        return mLevel <= level::INFO;
    }

    /** Check if logging statements of level WARN are enabled. */

    inline bool isWarnEnabled() const
    {
        return mLevel <= level::WARN;
    }

    /** Check if logging statements of level ERROR are enabled. */

    inline bool isErrorEnabled() const
    {
        return mLevel <= level::SERROR;
    }

    /** Check if logging statements of level FATAL are enabled. */

    inline bool isFatalEnabled() const
    {
        return mLevel <= level::FATAL;
    }

    /** Getter routine for the logging level of this object. */

    inline level::Level getLevel() const
    {
        return mLevel;
    }

    /** Getter routine for the effective logging level of
     this object. If level has not been set explicitly it
     will ask the ancestors for the level
     */

    level::Level getEffectiveLevel() const;

    /** Setter routine for the logging level of this object.

     \param level is the new logging level for this object
     \param force true specifies that this is an explicit set

     This routine will set implicitly the levels recursively
     for the descendants whose level has not been set explicitly.
     */

    void setLevel( const level::Level level, const bool force = true );

    /** Logging output for level TRACE. This routine should
     only be called if TraceEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void trace( SourceLocation loc, const std::string& msg ) = 0;

    /** Logging output for level DEBUG. This routine should
     only be called if isDebugEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void debug( SourceLocation loc, const std::string& msg ) = 0;

    /** Logging output for level INFO. This routine should
     only be called if isInfoEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void info( SourceLocation loc, const std::string& msg ) = 0;

    /** Logging output for level WARN. This routine should
     only be called if isWarnEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void warn( SourceLocation loc, const std::string& msg ) = 0;

    /** Logging output for level ERROR. This routine should
     only be called if isErrorEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void error( SourceLocation loc, const std::string& msg ) = 0;

    /** Logging output for level FATAL. This routine should
     only be called if isFatalEnabled() returns true.

     \param loc is the file location of the logging statement
     \param msg is the message to be printed

     Each derived class has to implement this routine. This
     abstract class does not handle output of logging at all.
     */

    virtual void fatal( SourceLocation loc, const std::string& msg ) = 0;

    /** Predicate that returns true if this logger is the root logger. */

    bool isRootLogger() const;

protected:

    /** Constructor for a logger.
     *
     \param name is the name of this logger (not full name)
     \param parent is pointer to the parent logger
     */

    Logger( const std::string& name, class Logger* parent );

    std::string mName; //!< name of the logger

    bool mSetFlag; //!< This flag indicates that level has been set explicitly.

    level::Level mLevel; //!< Current level of this logger.

    class Logger* const mParent; //!< points back to the parent logger, NULL for root

    std::vector<class Logger*> mSons; //!< sub loggers of this logger

private:

    Logger(); //!< Disable default logger

    Logger( const Logger& other );   // Disable default copy-constructor

    const Logger& operator=( const Logger& other ); // Disable default assignment operator
};

} /* end namespace logging */

} /* end namespace scai */
