set(C_SOURCE_FILE 
card.c
stack.c
graphic-context.c   
game.c 
r7/r7.c
qll/qll.c
)

add_library(cardGames ${C_SOURCE_FILE})
target_link_libraries(cardGames SDL2 SDL2_image)
include_directories(${SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})