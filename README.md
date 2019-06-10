[![Build Status](https://travis-ci.com/Hexik/safe_cast.svg?branch=master)](https://travis-ci.com/Hexik/safe_cast)
<a href="https://scan.coverity.com/projects/hexik-safe_cast">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/18567/badge.svg"/>
</a>
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/48e1ea1b8d774084a41b045afd853047)](https://www.codacy.com/app/Hexik/safe_cast?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Hexik/safe_cast&amp;utm_campaign=Badge_Grade)

# SafeCast

SafeCast is a simple C++ header only library checking integer conversion.
## Installation

Download the safecast.h and include it in your project.

## Usage

Replace static_cast<> by safe_cast<> and insert safecast<> to places that indicate a warning -Wconversion.
Use C++14 standard at least.

```c++
#include "safecast.h"

int16_t i16;
int32_t i32 = std::numeric_limits<int16_t>::max();

CHECK_NOTHROW( i16 = safe_cast<int16_t>( i32 ) );
i32++;

CHECK_THROWS( i16 = safe_cast<int16_t>( i32 ) );
CHECK( i16 != i32 );

```
Exceptions are used for overflow signaling, it is possible to use assert, logging, ...
## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

-   **[MIT license](http://opensource.org/licenses/mit-license.php)**
-   Copyright 2014-2019 © Miroslav Fontan