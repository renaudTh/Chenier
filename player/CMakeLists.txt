set(C_SOURCE_FILE 
player.c
)
add_executable(cardGamePlayer ${C_SOURCE_FILE})
target_link_libraries(cardGamePlayer cardGames)
include_directories(${SOURCE_DIR} ${SDL2_INCLUDE_DIRS})