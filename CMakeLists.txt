project(CardGames)
cmake_minimum_required(VERSION 3.12)

# Compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Specify the source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(TESTER_DIR "${CMAKE_SOURCE_DIR}/tester")
set(PLAYER_DIR "${CMAKE_SOURCE_DIR}/player")

# Build the project from the source directory
add_subdirectory(${SOURCE_DIR})
add_subdirectory(${TESTER_DIR}) 
add_subdirectory(${PLAYER_DIR})