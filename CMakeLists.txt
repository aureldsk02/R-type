cmake_minimum_required(VERSION 3.15)

# Intégrer vcpkg AVANT le project()
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(RType VERSION 1.0 LANGUAGES CXX)

# -------------------------------------------------------------
# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Vérifications spécifiques à Linux
if(UNIX AND NOT APPLE)
    message(STATUS "Linux detected: checking for system libraries")
    find_library(UDEV_LIB udev)
    if(NOT UDEV_LIB)
        message(WARNING 
        "libudev not found. SFML may require it for joystick support.\n"
        "Install with:\n"
        " Ubuntu/Debian: sudo apt-get install libudev-dev\n"
        " Fedora: sudo dnf install systemd-devel\n"
        " Arch: sudo pacman -S systemd"
        )
    endif()
endif()

#----------------------------------------------------------------
# Chercher les packages via vcpkg
find_package(SFML COMPONENTS system window graphics network REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Chercher libconfig avec gestion cross-platform
find_package(libconfig CONFIG QUIET)
if(NOT libconfig_FOUND)
    find_package(libconfig++ CONFIG QUIET)
endif()

# Debug: afficher les targets disponibles
message(STATUS "libconfig_FOUND: ${libconfig_FOUND}")
message(STATUS "libconfig++_FOUND: ${libconfig++_FOUND}")
get_property(imported_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "All imported targets: ${imported_targets}")

# Détecter automatiquement le bon target libconfig selon la plateforme
if(TARGET libconfig::config++)
    set(LIBCONFIG_TARGET libconfig::config++)
    message(STATUS "Using libconfig target: libconfig::config++")
elseif(TARGET libconfig++::libconfig++)
    set(LIBCONFIG_TARGET libconfig++::libconfig++)
    message(STATUS "Using libconfig target: libconfig++::libconfig++")
elseif(TARGET libconfig::libconfig++)
    set(LIBCONFIG_TARGET libconfig::libconfig++)
    message(STATUS "Using libconfig target: libconfig::libconfig++")
elseif(TARGET libconfig)
    set(LIBCONFIG_TARGET libconfig)
    message(STATUS "Using libconfig target: libconfig")
else()
    message(FATAL_ERROR "libconfig target not found. Available targets: ${imported_targets}")
endif()

find_package(OpenAL CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(Ogg CONFIG REQUIRED)
find_package(FLAC CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

#----------------------------------------------------------------
# Génération de la documentation avec Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else()
    message(STATUS "Doxygen not found, documentation will not be generated")
endif()

# -------------------------------------------------------------
# Options pour compiler client et/ou serveur
option(BUILD_CLIENT "Build the client executable" ON)
option(BUILD_SERVER "Build the server executable" ON)
option(BUILD_TESTS "Build the test executable" ON)

#  -------------------------------------------------------------
# Clang-Tidy
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found")
    endif()
endif()

# -------------------------------------------------------------
# Clang-Format
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES 
        "rtype_client/src/*.cpp" "rtype_client/include/*.hpp" 
        "rtype_server/src/*.cpp" "rtype_server/include/*.hpp")
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting all source files"
    )
endif()

# -------------------------------------------------------------
# Engine
file(GLOB_RECURSE ENGINE_SOURCES 
        "engine/src/*.cpp")

if(NOT ENGINE_SOURCES)
    message(FATAL_ERROR "No engine source files found in engine/src/")
endif()

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PRIVATE 
    ${CMAKE_SOURCE_DIR}/utility_classes/include 
    ${CMAKE_SOURCE_DIR}/engine/include
)

target_link_libraries(engine PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)

# -------------------------------------------------------------
# Client
if(BUILD_CLIENT)
    file(GLOB_RECURSE CLIENT_SOURCES 
        "rtype_graphicsClient/src/*.cpp" 
        "mains/client/main.cpp" 
        "utility_classes/src/*.cpp"
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/UI/components/src/*.cpp
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/UI/src/*.cpp
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/Network/ClientTCP/src/*.cpp
        )
    
    if(NOT CLIENT_SOURCES)
        message(FATAL_ERROR "No client source files found in rtype_graphicsClient/src/")
    endif()
    
    add_executable(r-type_client ${CLIENT_SOURCES})
    
    target_include_directories(r-type_client PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_graphicsClient/include 
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/Managers 
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/Network/ClientTCP/include
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/UI/components/include
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/UI/include
        ${CMAKE_SOURCE_DIR}/network/RTypeClient/UI/parsers/include
        ${CMAKE_SOURCE_DIR}/utility_classes/include 
        ${CMAKE_SOURCE_DIR}/engine/include
    )
    
    target_link_libraries(r-type_client PRIVATE 
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        asio::asio
        ${LIBCONFIG_TARGET}
        OpenAL::OpenAL
        Vorbis::vorbisenc Vorbis::vorbisfile Vorbis::vorbis
        Ogg::ogg
        FLAC::FLAC
        unofficial::sqlite3::sqlite3
        engine
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_client PRIVATE 
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -g>
        )
    endif()
endif()

# -------------------------------------------------------------
# Serveur
if(BUILD_SERVER)
    file(GLOB_RECURSE SERVER_SOURCES 
        "rtype_gameInstance/src/*.cpp" 
        "rtype_gameManager/src/*.cpp" 
        "mains/server/main.cpp" 
        "utility_classes/src/*.cpp"
        ${CMAKE_SOURCE_DIR}/network/RTypeServer/Auth/src/*.cpp
        ${CMAKE_SOURCE_DIR}/network/RTypeServer/Network/ServerTCP/src/*.cpp
        )

    if(NOT SERVER_SOURCES)
        message(FATAL_ERROR "No server source files found in rtype_server/src/")
    endif()
    
    add_executable(r-type_server ${SERVER_SOURCES})
    
    target_include_directories(r-type_server PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_gameInstance/include 
        ${CMAKE_SOURCE_DIR}/rtype_gameManager/include 
        ${CMAKE_SOURCE_DIR}/utility_classes/include 
        ${CMAKE_SOURCE_DIR}/engine/include
        ${CMAKE_SOURCE_DIR}/network/RTypeServer/Auth/include
        ${CMAKE_SOURCE_DIR}/network/RTypeServer/Managers
        ${CMAKE_SOURCE_DIR}/network/RTypeServer/Network/ServerTCP/include
    )
    
    target_link_libraries(r-type_server PRIVATE 
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        asio::asio
        unofficial-sodium::sodium
        ${LIBCONFIG_TARGET}
        OpenAL::OpenAL
        Vorbis::vorbisenc Vorbis::vorbisfile Vorbis::vorbis
        Ogg::ogg
        FLAC::FLAC
        unofficial::sqlite3::sqlite3
        engine
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_server PRIVATE 
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -g>
        )
    endif()
endif()

# -------------------------------------------------------------
# Tests
if(BUILD_TESTS)
    file(GLOB_RECURSE TESTS_SOURCES 
        "rtype_server/tests/*.cpp"  
        "rtype_client/tests/*.cpp" 
        "utility_classes/tests/*.cpp" 
        "engine/tests/*.cpp" 
        "rtype_server/src/*.cpp"  
        "rtype_client/src/*.cpp" 
        "utility_classes/src/*.cpp" 
        "engine/src/*.cpp")
    
    if(NOT TESTS_SOURCES)
        message(FATAL_ERROR "No test source files found")
    endif()

    add_executable(r-type_tests ${TESTS_SOURCES})
    
    target_include_directories(r-type_tests PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_server/include  
        ${CMAKE_SOURCE_DIR}/rtype_client/include 
        ${CMAKE_SOURCE_DIR}/utility_classes/include 
        ${CMAKE_SOURCE_DIR}/engine/include
    )
    
    target_link_libraries(r-type_tests PRIVATE 
        GTest::gtest 
        GTest::gtest_main
        sfml-system sfml-window sfml-graphics sfml-network sfml-audio
        asio::asio
        ${LIBCONFIG_TARGET}
        OpenAL::OpenAL
        Vorbis::vorbisenc Vorbis::vorbisfile Vorbis::vorbis
        Ogg::ogg
        FLAC::FLAC
        unofficial::sqlite3::sqlite3
        engine
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_tests PRIVATE 
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -g>
        )
    endif()
    
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(r-type_tests)
endif()

# -------------------------------------------------------------
# Apocalypse
add_executable(apocalypse ${CMAKE_SOURCE_DIR}/apocalypse/main.cpp)

target_link_libraries(apocalypse PRIVATE 
    engine
)

target_include_directories(apocalypse PRIVATE 
    ${CMAKE_SOURCE_DIR}/engine/include
)
