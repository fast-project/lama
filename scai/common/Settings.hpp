/**
 * @file Settings.hpp
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
 * @brief Utilities for LAMA settings
 * @author Thomas Brandes
 * @date 19.06.2013
 * @since 1.0.1
 */
#pragma once

// std
#include <string>
#include <vector>

namespace scai
{

namespace common
{

//class Communicator;

/**
 *  This singleton class provides methods to query environment variables.
 *
 *  Note: This should be the only module to access environment variables
 *        directly.
 */

class Settings
{
public:

    /** Set a flag by value of its environment variable
     *
     *  @param[out]  flag is boolean variable that will be set
     *  @param[in]   envVarName is name of the environment variable
     *  @return      true if environment variable has been used to set flag
     */
    static bool getEnvironment( bool& flag, const char* envVarName );

    /** Set a integer by value of its environment variable
     *
     *  @param[out]  val is integer variable that will be set
     *  @param[in]   envVarName is name of the environment variable
     *  @return      true if environment variable has been used to set flag
     */
    static bool getEnvironment( int& val, const char* envVarName );

    /** Set a string by value of its environment variable
     *
     *  @param[out]  val is string that will be set
     *  @param[in]   envVarName is name of the environment variable
     *  @return      true if environment variable was set and provided an integer vlaue
     */
    static bool getEnvironment( std::string& val, const char* envVarName );

    /** Get tokenized string from an environment variable 
     *
     *  @param[out] vals is a vector of separated strings from the environment varialbe
     *  @param[in]  envVarName is name of the environment variable
     *  @param[in]  separator is the character used to separate
     *  @return     true if environment variable was set
     */
    static bool getEnvironment( std::vector<std::string>& vals, const char* envVarName, const char separator );

    /** Define an environment variable */

    static void putEnvironment( const char* envVarName, const char* val, bool replace = true );

    /** Define an environment variable */

    static void putEnvironment( const char* envVarName, const int val, bool replace = true );

    /** Help routine to tokenize a string by a given separator
     *
     *  @param[out] values is a vector of separated strings from the input string
     *  @param[in]  input is a string that will be tokenized
     *  @param[in]  seperator is the character used to separate
     *
     */
    static void tokenize( std::vector<std::string>& values, const std::string& input, const char seperator );

private:

    Settings();

    /** convert the string value to a boolean value, name only used for messages.
     *
     *  @param[out]  flag is boolean variable that will be set
     *  @param[in]   value is string to be converted
     *  @return      true if string could be converted, false if no legal value has been found
     */

    static bool convertYesNoString( bool& flag, const char* value );

    /** convert the string value to an int value
     *
     *  @param[out]  number is variable that will be set
     *  @param[in]   value is string to be converted
     *  @return      true if string could be converted, false if no legal value has been found
     */

    static bool convertValue( int& number, const char* value );

    static bool init();
};

} /* end namespace common */

} /* end namespace scai */
