
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
# -Wpadded only reports struct alignment, it is noise for a header only cast library
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( selfTest PRIVATE -Weverything -Wno-c++98-compat-pedantic -Wno-padded )
endif()

# configure unit tests via CTest
add_test(NAME RunTests COMMAND $<TARGET_FILE:selfTest>)
