cmake_minimum_required(VERSION 3.16)

# Project configuration
project(RType 
    VERSION 1.0.0 
    DESCRIPTION "R-Type Multiplayer Game"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies managed by vcpkg
find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Sub-projects
add_subdirectory(shared)
add_subdirectory(ecs-engine)
add_subdirectory(r-type-server)
add_subdirectory(r-type-client)

# Build info
message(STATUS "========================================")
message(STATUS "R-Type Project Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "========================================")
