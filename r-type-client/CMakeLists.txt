cmake_minimum_required(VERSION 3.16)
project(RTypeClient VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
find_package(Threads REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Client source files
set(CLIENT_SOURCES
    src/main.cpp
    src/GameClient.cpp
    src/NetworkClient.cpp
    src/Scene.cpp
    src/Game.cpp
    src/EcsWrapper.cpp
    src/Chat.cpp
    src/FontManager.cpp
    src/menu.cpp
    src/GameModeMenu.cpp
    src/MultiplayerForm.cpp
    src/SettingsMenu.cpp
    src/HelpMenu.cpp
    src/RoomMenu.cpp
    src/RoomListMenu.cpp
    src/RoomLobby.cpp
    src/graphics/Display_Handler.cpp
    src/graphics/Input_Handler.cpp
    src/graphics/Audio_Handler.cpp
)

# Create executable
add_executable(r-type-client ${CLIENT_SOURCES})

# Include directories
target_include_directories(r-type-client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ecs-engine/include
    ${CMAKE_SOURCE_DIR}/shared/include
)

# Compile definitions
target_compile_definitions(r-type-client PRIVATE
    ASIO_STANDALONE
)

# Compiler options
if(MSVC)
    target_compile_options(r-type-client PRIVATE /W4)
else()
    target_compile_options(r-type-client PRIVATE -Wall -Wextra -Wpedantic -g3)
endif()

# Link libraries
target_link_libraries(r-type-client PRIVATE
    ecs_engine
    Threads::Threads
    raylib
    nlohmann_json::nlohmann_json
)

# Set output directory
set_target_properties(r-type-client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy resources after build
add_custom_command(TARGET r-type-client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    ${CMAKE_BINARY_DIR}/bin/res
    COMMENT "Copying resources to build directory"
)

# Installation
install(TARGETS r-type-client
    RUNTIME DESTINATION bin
)

install(DIRECTORY res/
    DESTINATION share/r-type/res
)
