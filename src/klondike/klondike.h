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
	int index_from;
	struct _KlondikeGameLegalMove *next;

} KlondikeGameLegalMove;

KlondikeGameLegalMove *klondike_game_legal_move_new(KlondikeGameMoveType type, Stack *from, Stack *to);
KlondikeGameLegalMove *klondike_game_talon_move(KlondikeGame *kg);
KlondikeGameLegalMove *klondike_game_pile_move(KlondikeGame *kg);
KlondikeGameLegalMove *klondike_game_suite_moves(KlondikeGame *kg);
bool klondike_game_is_to_build_legal_move(const Card *from, Stack *to);

void klondike_game_legal_move_append(KlondikeGameLegalMove **head, KlondikeGameLegalMove *to_append);
KlondikeGame *klondike_game_new();
KlondikeGameLegalMove *klondike_game_build_moves(KlondikeGame *kg);
bool klondike_game_winning_init(void *game);
bool klondike_game_losing_init(void *game);
GameActionResult klondike_game_play_card(void *game);
GameActionResult klondike_game_iterate(void *game);
bool klondike_game_ended(void *game);
bool klondike_game_won(void *game);
void klondike_game_destroy(void *game);
#endif // KLONDIKE_H