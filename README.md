[![CI](https://github.com/Hexik/safe_cast/actions/workflows/ci.yml/badge.svg)](https://github.com/Hexik/safe_cast/actions/workflows/ci.yml)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/48e1ea1b8d774084a41b045afd853047)](https://www.codacy.com/app/Hexik/safe_cast?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Hexik/safe_cast&amp;utm_campaign=Badge_Grade)

# SafeCast

SafeCast is a simple C++ header only library checking integer conversion.
## Installation

Download the safecast.h and include it in your project.

Or consume it as a CMake target:

```cmake
include(FetchContent)
FetchContent_Declare(SafeCast GIT_REPOSITORY https://github.com/Hexik/safe_cast.git GIT_TAG master)
FetchContent_MakeAvailable(SafeCast)

target_link_libraries(myapp PRIVATE SafeCast::SafeCast)
```

After `cmake --install`, `find_package(SafeCast REQUIRED)` provides the same `SafeCast::SafeCast` target.

## Usage

Replace static_cast<> by safe_cast<> and insert safecast<> to places that indicate a warning -Wconversion.
Use C++14 standard at least.

```c++
#include "safecast.h"

int32_t i32 = std::numeric_limits<int16_t>::max();

int16_t i16 = safe_cast<int16_t>( i32 ); // fits, no exception

try {
    i16 = safe_cast<int16_t>( i32 + 1 ); // throws
} catch( const SafeCastException& e ) {
    std::cerr << e.what() << '\n'; // signed to signed 32768 limit: 32767
}
```

Enums are resolved to their underlying type on both sides, so a negative enumerator
cannot silently slip into a narrower type.

Exceptions are used for overflow signaling, it is possible to use assert, logging, ...
Define `NO_SAFECAST` to compile all checks away, `safe_cast<>` then degrades to `static_cast<>`.
## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

-   **[MIT license](http://opensource.org/licenses/mit-license.php)**
-   Copyright 2014-2026 (c) Miroslav Fontan