set(C_SOURCE_FILE 
card.c
stack.c  
game.c 
r7/r7.c
qll/qll.c
c4/c4.c
parser/parser.c
up-down/up-down.c
klondike/klondike.c
)

add_library(cardGames ${C_SOURCE_FILE})
target_link_libraries(cardGames)
include_directories(${CHENIER_SOURCE_DIR})