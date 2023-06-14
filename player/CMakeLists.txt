set(C_SOURCE_FILE 
game-render.c
graphic-context.c
player.c
)

add_executable(cardGamePlayer ${C_SOURCE_FILE})
target_link_libraries(cardGamePlayer cardGames SDL2 SDL2_image)
include_directories(${INCLUDE_DIR} ${SOURCE_DIR} ${PLAYER_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})