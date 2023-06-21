#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "game.h"
#include "stack.h"

typedef struct KlondikeGame {

	Stack *pile;
	Stack *talon;
	Stack *suite[4];
	Stack *build[7];

} KlondikeGame;

typedef enum KlondikeGameMoveType {
	BuildToBuild,
	BuildToSuit,
	TalonToBuild,
	TalonToSuite,
	PileToTalon,
	SuiteToBuild,
	Redeal,
} KlondikeGameMoveType;

typedef struct _KlondikeGameLegalMove {

	KlondikeGameMoveType type;
	Stack *from;
	Stack *to;
	struct _KlondikeGameLegalMove *next;

} KlondikeGameLegalMove;

KlondikeGameLegalMove *klondike_game_legal_move_new(KlondikeGameMoveType type, const Stack *from, const Stack *to);

KlondikeGame *klondike_game_new();
bool klondike_game_winning_init(void *game);
bool klondike_game_losing_init(void *game);
GameActionResult klondike_game_play_card(void *game);
GameActionResult klondike_game_iterate(void *game);
bool klondike_game_ended(void *game);
bool klondike_game_won(void *game);
void klondike_game_destroy(void *game);
#endif // KLONDIKE_H