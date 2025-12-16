cmake_minimum_required(VERSION 3.16)
project(RTypeServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Server source files
set(SERVER_SOURCES
    src/main.cpp
    src/NetworkManager.cpp
    src/GameRoom.cpp
    src/RoomManager.cpp
    src/Broadcast.cpp
    src/signal.cpp
)

# Create executable
add_executable(r-type-server ${SERVER_SOURCES})

# Include directories
target_include_directories(r-type-server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ecs-engine/include
    ${CMAKE_SOURCE_DIR}/shared/include
)

# Compile definitions
target_compile_definitions(r-type-server PRIVATE
    ASIO_STANDALONE
)

# Compiler options
if(MSVC)
    target_compile_options(r-type-server PRIVATE /W4)
else()
    target_compile_options(r-type-server PRIVATE -Wall -Wextra -Wpedantic -g3)
endif()

# Link libraries
target_link_libraries(r-type-server PRIVATE
    ecs_engine
    Threads::Threads
    nlohmann_json::nlohmann_json
)

# Set output directory
set_target_properties(r-type-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Installation
install(TARGETS r-type-server
    RUNTIME DESTINATION bin
)
