/*
 * Copyright (C) 2014-2026 Miroslav Fontan
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/*
 * Second translation unit including safecast.h, it guards against ODR
 * violations (non-inline definitions in the header).
 */

#include <catch2/catch_amalgamated.hpp>
#include "safecast.h"

TEST_CASE( "second translation unit", "[All]" )
{
    CHECK( safe_cast<int8_t>( 42 ) == 42 );
    CHECK_THROWS_AS( safe_cast<int8_t>( 1000 ), SafeCastException );
}
