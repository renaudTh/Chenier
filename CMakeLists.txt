project(CardGames LANGUAGES C)
cmake_minimum_required(VERSION 3.12)

# Compiler flags

# Specify the source directory
set(CHENIER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(CHENIER_TESTER_DIR "${CMAKE_SOURCE_DIR}/tester")
set(CHENIER_PLAYER_DIR "${CMAKE_SOURCE_DIR}/player")
set(CHENIER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

include_directories(${CHENIER_INCLUDE_DIR})
# Build the project from the source directory
add_subdirectory(${CHENIER_SOURCE_DIR})

add_subdirectory(external/cunit EXCLUDE_FROM_ALL)
add_subdirectory(${CHENIER_TESTER_DIR}) 


add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

set(SDL2IMAGE_INSTALL OFF)
add_subdirectory(external/SDL_image)

add_subdirectory(${CHENIER_PLAYER_DIR})