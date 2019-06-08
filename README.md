# SafeCast

SafeCast is a simple C++ header only library checking integer conversion.
## Installation

Download the safecast.h and include it in your project.


## Usage

Replace static_cast<> by safe_cast<> and insert safecast to places that indicate a warning -Wconversion

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
[MIT](https://choosealicense.com/licenses/mit/)