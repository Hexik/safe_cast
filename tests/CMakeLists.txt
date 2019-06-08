
add_library(test_main OBJECT test_main.cpp)

add_executable(selfTest test_safecast.cpp $<TARGET_OBJECTS:test_main>)
target_include_directories(selfTest PRIVATE ${HEADER_DIR})

# Add per compiler options
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( selfTest PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -Wmissing-declarations -Werror )
endif()
# Clang specific options go here
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( selfTest PRIVATE -Wweak-vtables -Wexit-time-destructors -Wmissing-noreturn )
endif()

# configure unit tests via CTest
add_test(NAME RunTests COMMAND $<TARGET_FILE:selfTest>)
