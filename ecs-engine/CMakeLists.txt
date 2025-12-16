cmake_minimum_required(VERSION 3.16)
project(EcsEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ECS Engine source files
set(ECS_SOURCES
    src/composants.cpp
    src/systems/MovementSystem.cpp
    src/systems/CollisionSystem.cpp
    src/systems/HealthSystem.cpp
    src/systems/ShootingSystem.cpp
    src/systems/AiSystem.cpp
    src/systems/SpawnSystem.cpp
    src/systems/DestroySystem.cpp
    src/systems/NetworkSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/Scene_loader.cpp
)

# Create static library
add_library(ecs_engine STATIC ${ECS_SOURCES})

# Public include directories
target_include_directories(ecs_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ecs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/network>
    $<INSTALL_INTERFACE:include>
)

# Compiler options
if(MSVC)
    target_compile_options(ecs_engine PRIVATE /W4)
else()
    target_compile_options(ecs_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS ecs_engine
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)
