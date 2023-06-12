set(C_SOURCE_FILE 
player.c
)
add_executable(cardGamePlayer ${C_SOURCE_FILE})
target_link_libraries(cardGamePlayer cardGames SDL2 SDL2_image -fsanitize=address,undefined)
include_directories(${SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})