set(C_SOURCE_FILE 
card-stack-tester.c
r7-tester.c
klondike-tester.c
tester.c
)

add_executable(cardGameTester ${C_SOURCE_FILE})
target_link_libraries(cardGameTester cunit cardGames)
include_directories(${INCLUDE_DIR} ${SOURCE_DIR})