set(C_SOURCE_FILE 
game-render.c
graphic-context.c
player.c
)

add_executable(cardGamePlayer ${C_SOURCE_FILE})
target_link_libraries(cardGamePlayer PRIVATE cardGames SDL2 SDL2_image pthread)
include_directories(${CHENIER_INCLUDE_DIR} ${CHENIER_SOURCE_DIR} ${CHENIER_PLAYER_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})