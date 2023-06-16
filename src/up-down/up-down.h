#ifndef UPDOWN_H
#define UPDWON_H

#include "game.h"
#include "stack.h"

typedef struct UpDownGame_t {

	Stack *build[4];
	Stack *stock[4];
	Stack *deck;
	bool end;

} UpDownGame;

UpDownGame *up_down_game_new();

bool up_down_game_init(void *game);
GameActionResult up_down_game_play_card(void *game);
bool up_down_game_winning_condition(void *game);
bool up_down_game_ending_condition(void *game);
GameActionResult up_down_game_iterate(void *game);
void up_down_game_destroy(UpDownGame *rg);

extern CardGame up_down_winning;

#endif