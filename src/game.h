#ifndef GAME_H
#define GAME_H

#include "graphic-context.h"

typedef bool (*game_init)(void *);
typedef bool (*game_play_card)(void *);
typedef bool (*game_iterate)(void *);
typedef bool (*game_ended)(void *);
typedef bool (*game_won)(void *);

typedef struct CardGame {
	char *name;
	void *game;
	game_init init;
	game_play_card play_card;
	game_iterate iterate;
	game_ended ended;
	game_won won;
	render_fct render;
} CardGame;
bool card_game_play(CardGame *cg);
bool card_game_play_graphic(GraphicContext *ctx, CardGame *cg);

extern CardGame r7_game;

#endif // GAME_H