#ifndef QLL_H
#define QLL_H

#include "game.h"
#include "stack.h"

typedef struct QllGame {

	int nb_card;
	Stack *deck;
	Stack **table;
	int line_size;

} QllGame;

QllGame *qll_game_new(int nb_cards);
bool qll_game_winning_init(void *game);
bool qll_game_losing_init(void *game);
bool qll_game_reinit(void *game);
GameActionResult qll_game_play_card(void *game);
GameActionResult qll_game_iterate(void *game);
bool qll_game_ended(void *game);
bool qll_game_won(void *game);
void qll_game_destroy(void *game);

extern CardGame qll_win;
extern CardGame qll_lose;

#endif // QLL_H