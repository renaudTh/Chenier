#ifndef R7_H
#define R7_H

#include "game.h"
#include "stack.h"

#define NB_R7_BUILD_STACK 4
typedef struct R7Game_t {

	Stack *build[NB_R7_BUILD_STACK];
	Stack *deck;
	Stack *bin;
	int attempt_nb;
} R7Game;

R7Game *r7_game_new();

void r7_init_without_deck(R7Game *rg);
bool r7_init_winning_game_in_one_attempt(void *game);
bool r7_init_losing_game(void *game);
bool r7_init_winning_game_in_two_attempts(void *game);

bool r7_game_init(void *game);
bool r7_game_reinit(void *game);
GameActionResult r7_game_play_card(void *game);
bool r7_game_winning_condition(void *game);
bool r7_game_ending_condition(void *game);
GameActionResult r7_game_iterate(void *game);
bool r7_game_main_loop(R7Game *rg);
void r7_game_destroy(R7Game *rg);

extern CardGame r7_winning;
extern CardGame r7_losing;
extern CardGame r7_normal;

#endif