cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MultiplayerLinesOfAction)
project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED False)

option(BuildClient "BuildClient" OFF)
option(BuildServer "BuildServer" OFF)

add_subdirectory(libs/spdlog)
add_subdirectory(common)

if( BuildClient )
    message("Building client for Windows system")
    add_subdirectory(client)
endif()

if( BuildServer )
    message("Building server for Linux system")
    add_subdirectory(server)
endif()

unset(BuildClient CACHE)
unset(BuildServer CACHE)

# lots of warnings and all warnings as errors
add_compile_options(-Wall -Wextra -pedantic -Werror)