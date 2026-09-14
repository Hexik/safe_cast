
add_library(test_main OBJECT test_main.cpp)

# test_odr.cpp is a second translation unit, it catches ODR violations in safecast.h
add_executable(selfTest test_safecast.cpp test_odr.cpp $<TARGET_OBJECTS:test_main>)
target_link_libraries(selfTest PRIVATE SafeCast::SafeCast)
# Catch2 is third party code, SYSTEM keeps its warnings out of our -Werror build
target_include_directories(selfTest SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add per compiler options
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( selfTest PRIVATE -Wall -Wextra -Wconversion -Wunreachable-code -Wpedantic -Wmissing-declarations -Werror )
endif()
# Clang specific options go here, only for braves
# -Weverything is not meant for production use, every new Clang release adds new
# warnings, so the noisy ones which say nothing about our code are disabled:
#   -Wpadded        only reports struct alignment
#   -Wweak-vtables  inherent to a header only library, SafeCastException cannot have
#                   an out-of-line virtual method to anchor its vtable
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( selfTest PRIVATE
        -Weverything
        -Wno-c++98-compat-pedantic
        -Wno-padded
        -Wno-weak-vtables )
endif()
# AppleClang complains about /usr/local/include coming from the build environment
if ( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
    target_compile_options( selfTest PRIVATE -Wno-poison-system-directories )
endif()

# configure unit tests via CTest
add_test(NAME RunTests COMMAND $<TARGET_FILE:selfTest>)
