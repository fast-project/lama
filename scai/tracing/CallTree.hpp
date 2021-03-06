/**
 * @file CallTree.hpp
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
 * @brief Definition of static class that provides routines to generate a Call Tree.
 *
 * @author Thomas Brandes
 * @date 03.07.2011
 */

#pragma once

// local library
#include <scai/tracing/RegionEntry.hpp>
#include <scai/tracing/Counters.hpp>

// internal scai libraries
#include <scai/logging.hpp>

// std
#include <vector>

namespace scai
{

namespace tracing
{

class CallTree
{
public:

    static void enter( const int region_id, RegionEntry& region, const CounterArray& startValues );

    static void leave( const int region_id, const RegionEntry& region, const CounterArray& stopValues );

    static void finish();

private:

    static std::vector<int> theCallStack;

    SCAI_LOG_DECL_STATIC_LOGGER( logger )
};

} /* end namespace tracing */

} /* end namespace scai */
