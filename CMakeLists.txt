cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME MultiplayerLinesOfAction)
project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BuildClient "BuildClient" OFF)
option(BuildServer "BuildServer" OFF)

add_subdirectory(libs/spdlog)
add_subdirectory(common)

if( BuildClient )
    message("Building client for Windows system")
    add_subdirectory(client)
    set(BUILD_SHARED_LIBS FALSE)
    add_subdirectory(libs/SFML) 
endif()

if( BuildServer )
    message("Building server for Linux system")
    add_subdirectory(server)
endif()

unset(BuildClient CACHE)
unset(BuildServer CACHE)

if (MSVC)
    target_compile_options( ${PROJECT_NAME} PRIVATE /W4 )
    target_compile_options( common PRIVATE /W4 )
else()
    target_compile_options( ${PROJECT_NAME} PRIVATE -Wall )
    target_compile_options( common PRIVATE -Wall )
endif()