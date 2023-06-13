#ifndef QLL_H
#define QLL_H

#include "../stack.h"
#include <stdlib.h>

typedef struct QllGame {

	int nb_card;
	Stack *deck;
	Stack **table;
	int line_size;

} QllGame;

QllGame *qll_game_new(int nb_cards);
bool qll_game_init(void *game);
bool qll_game_play_card(void *game);
bool qll_game_iterate(void *game);
bool qll_game_ended(void *game);
bool qll_game_won(void *game);

#endif // QLL_H