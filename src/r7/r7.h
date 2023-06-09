#ifndef R7_H
#define R7_H

#include "../stack.h"

#define NB_R7_BUILD_STACK 4
typedef struct R7Game_t {

	Stack *build[NB_R7_BUILD_STACK];
	Stack *deck;
	Stack *bin;
	int attempt_nb;
} R7Game;

R7Game *r7_game_new();
void r7_game_init(R7Game *rg);
bool r7_game_play_card(R7Game *rg);
bool r7_game_main_loop(R7Game *rg);
void r7_game_destroy(R7Game *rg);
#endif