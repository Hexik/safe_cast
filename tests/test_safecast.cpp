/*
 * Copyright (C) 2014-2019 Miroslav Fontan
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "catch2/catch.hpp"
#include "safecast.h"

#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

TEST_CASE( "unsigned -> signed", "[All]" )
{
    /* unsigned -> signed, overflow */
    CHECK_THROWS( safe_cast<int8_t>( std::numeric_limits<uint8_t>::max() ) );

    /* unsigned -> signed, no overflow */
    CHECK_NOTHROW( safe_cast<int16_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<int32_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<int64_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<ssize_t>( std::numeric_limits<uint8_t>::max() ) );

    /* unsigned -> signed, overflow */
    CHECK_THROWS( safe_cast<int8_t>( std::numeric_limits<uint32_t>::max() ) );
    CHECK_THROWS( safe_cast<int16_t>( std::numeric_limits<uint32_t>::max() ) );
    CHECK_THROWS( safe_cast<int32_t>( std::numeric_limits<uint32_t>::max() ) );
    /* unsigned -> signed, no overflow */
    CHECK_NOTHROW( safe_cast<int64_t>( std::numeric_limits<uint32_t>::max() ) );
}

TEST_CASE( "unsigned -> unsigned", "[All]" )
{
    /* unsigned -> unsigned, no overflow */
    CHECK_NOTHROW( safe_cast<uint8_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<uint16_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<uint32_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<uint64_t>( std::numeric_limits<uint8_t>::max() ) );
    CHECK_NOTHROW( safe_cast<size_t>( std::numeric_limits<uint8_t>::max() ) );

    /* unsigned -> unsigned, overflow */
    CHECK_THROWS( safe_cast<uint8_t>( std::numeric_limits<uint64_t>::max() ) );
    CHECK_THROWS( safe_cast<uint16_t>( std::numeric_limits<uint64_t>::max() ) );
    CHECK_THROWS( safe_cast<uint32_t>( std::numeric_limits<uint64_t>::max() ) );
    if( sizeof( size_t ) >= 8 ) {
        /* unsigned -> unsigned, no overflow on 64bits */
        CHECK_NOTHROW( safe_cast<size_t>( std::numeric_limits<uint64_t>::max() ) );
    } else {
        /* unsigned -> unsigned, overflow on 32bits */
        CHECK_THROWS( safe_cast<size_t>( std::numeric_limits<uint64_t>::max() ) );
    }
    /* unsigned -> unsigned, no overflow */
    CHECK_NOTHROW( safe_cast<uint64_t>( std::numeric_limits<uint64_t>::max() ) );
}

TEST_CASE( "signed -> unsigned", "[All]" )
{
    /* signed -> unsigned, overflow */
    CHECK_THROWS( safe_cast<uint8_t>( ( -1 ) ) );
    CHECK_THROWS( safe_cast<uint16_t>( ( -1 ) ) );
    CHECK_THROWS( safe_cast<uint32_t>( ( -1 ) ) );
    CHECK_THROWS( safe_cast<uint64_t>( ( -1 ) ) );
    CHECK_THROWS( safe_cast<size_t>( ( -1 ) ) );
}

TEST_CASE( "signed -> signed", "[All]" )
{
    /* signed -> signed, overflow */
    CHECK_THROWS( safe_cast<int8_t>( std::numeric_limits<int32_t>::min() ) );
    CHECK_THROWS( safe_cast<int16_t>( std::numeric_limits<int32_t>::min() ) );
    /* signed -> signed, no overflow */
    CHECK_NOTHROW( safe_cast<int32_t>( std::numeric_limits<int32_t>::min() ) );
    CHECK_NOTHROW( safe_cast<int64_t>( std::numeric_limits<int32_t>::min() ) );
}

TEST_CASE( "narrow -> wide", "[All]" )
{
    /* always works (no check done) */
    CHECK_NOTHROW( safe_cast<int64_t>( std::numeric_limits<int32_t>::min() ) );
    CHECK_NOTHROW( safe_cast<int32_t>( std::numeric_limits<int16_t>::min() ) );
    CHECK_NOTHROW( safe_cast<int16_t>( std::numeric_limits<int8_t>::min() ) );
    CHECK_NOTHROW( safe_cast<int8_t>( std::numeric_limits<int8_t>::min() ) );
    CHECK_NOTHROW( safe_cast<ssize_t>( std::numeric_limits<int8_t>::max() ) );
}

TEST_CASE( "enum default", "[All]" )
{
    enum eDefault { OK = std::numeric_limits<int32_t>::max() };
    /* enum is an int by default */
    CHECK_THROWS( safe_cast<int8_t>( eDefault::OK ) );
    CHECK_THROWS( safe_cast<int16_t>( eDefault::OK ) );
    CHECK_NOTHROW( safe_cast<int32_t>( eDefault::OK ) );
    CHECK_NOTHROW( safe_cast<int64_t>( eDefault::OK ) );
}

TEST_CASE( "info loss", "[All]" )
{
    int16_t i16;
    int32_t i32 = std::numeric_limits<int16_t>::max();

    CHECK_NOTHROW( i16 = safe_cast<int16_t>( i32 ) );
    i32++;

    CHECK_THROWS( i16 = safe_cast<int16_t>( i32 ) );
    CHECK( i16 != i32 );
}
