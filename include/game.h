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

typedef enum CardGameType {
	CardGameTypeR7,
	CardGameTypeQLL,
	CardGameTypeC4,
	CardGameTypeUpDown,
	CardGameTypeKlondike,

} CardGameType;

typedef struct CardGame {
	char *name;
	void *game;
	CardGameType type;
	game_init init;
	game_play_card play_card;
	game_iterate iterate;
	game_ended ended;
	game_won won;
} CardGame;

bool card_game_play(CardGame *cg);
void card_game_new(CardGame *game);
void card_game_destroy(CardGame *game);

#endif // GAME_H