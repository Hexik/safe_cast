cmake_minimum_required(VERSION 3.0)

project(SafeCast LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

# Provide path for scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# define some folders
set(SAFECAST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SELF_TEST_DIR ${SAFECAST_DIR}/tests)
set(HEADER_DIR ${SAFECAST_DIR}/src)

# Prepare "SafeCast" library for other executables
add_library(SafeCast::SafeCast IMPORTED INTERFACE)
set_property(TARGET SafeCast::SafeCast PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${HEADER_DIR}")

enable_testing()
add_subdirectory(tests)

# add SafeCast as a 'linkable' target
add_library(SafeCast INTERFACE)
add_library(SafeCast::SafeCast ALIAS SafeCast)

