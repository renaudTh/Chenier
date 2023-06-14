set(C_SOURCE_FILE 
card.c
stack.c  
game.c 
r7/r7.c
qll/qll.c
)

add_library(cardGames ${C_SOURCE_FILE})
target_link_libraries(cardGames)
include_directories(${SOURCE_DIR})