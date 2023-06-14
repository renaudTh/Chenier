#ifndef C4_H
#define C4_H

#include "game.h"
#include "stack.h"

typedef struct C4Game {
	Stack *build[4];
	Stack *deck;
	int last_build_played;
	bool is_ended;
	bool is_win;
} C4Game;

C4Game *c4_game_new();
bool c4_game_init(void *game);
GameActionResult c4_game_play_card(void *game);
bool c4_game_winning_condition(void *game);
bool c4_game_ending_condition(void *game);
GameActionResult c4_game_iterate(void *game);
void c4_game_destroy(C4Game *cg);

#endif // C4_H