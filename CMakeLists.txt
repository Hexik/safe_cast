cmake_minimum_required(VERSION 3.16)

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
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# define some folders
set(SAFECAST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SELF_TEST_DIR ${SAFECAST_DIR}/tests)
set(HEADER_DIR ${SAFECAST_DIR}/src)

# SafeCast is header only, expose it as a 'linkable' interface target
add_library(SafeCast INTERFACE)
add_library(SafeCast::SafeCast ALIAS SafeCast)

target_include_directories(SafeCast INTERFACE
    $<BUILD_INTERFACE:${HEADER_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(SafeCast INTERFACE cxx_std_14)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

#
# Installation, makes find_package(SafeCast) work for consumers
#
install(FILES ${HEADER_DIR}/safecast.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS SafeCast EXPORT SafeCastTargets)
install(EXPORT SafeCastTargets
    NAMESPACE SafeCast::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SafeCast)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/CMake/SafeCastConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SafeCastConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SafeCast)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SafeCastConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SafeCastConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SafeCastConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SafeCast)
