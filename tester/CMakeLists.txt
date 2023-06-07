set(C_SOURCE_FILE 
card-stack-tester.c
card-game-tester.c
)

add_executable(cardGameTester ${C_SOURCE_FILE})
target_link_libraries(cardGameTester cunit cardGames)
include_directories(${SOURCE_DIR})