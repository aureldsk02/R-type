cmake_minimum_required(VERSION 3.16)
project(RTypeShared VERSION 1.0.0 LANGUAGES CXX)

# This is a header-only library for shared protocol definitions
add_library(shared INTERFACE)

target_include_directories(shared INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Installation
install(DIRECTORY include/
    DESTINATION include
)
