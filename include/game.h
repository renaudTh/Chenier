#ifndef GAME_H
#define GAME_H

#include <inttypes.h>
#include <stdbool.h>

typedef struct GameActionResult {
	uint8_t stateChanged : 1;
	uint8_t iterate : 1;
} GameActionResult;

typedef bool (*game_init)(void *);
typedef GameActionResult (*game_play_card)(void *);
typedef GameActionResult (*game_iterate)(void *);
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
} CardGame;

typedef enum CardGameType {
	CardGameTypeR7,
	CardGameTypeQLL,
} CardGameType;

bool card_game_play(CardGame *cg);

extern CardGame r7_game;
extern CardGame qll_game;

#endif // GAME_H