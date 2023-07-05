#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "game.h"
#include "stack.h"

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
	uint8_t score;
	uint8_t priority;
	struct _KlondikeGameLegalMove *next;

} KlondikeGameLegalMove;

typedef struct KlondikeGame {

	Stack *pile;
	Stack *talon;
	Stack *suite[4];
	Stack *build[7];
	int max_move_score;
	int max_move_priority;
	KlondikeGameLegalMove *moves_list;

	bool dead_end;
	bool lose;
} KlondikeGame;

void klondike_game_assign_priority(KlondikeGame *kg, KlondikeGameLegalMove *move);

KlondikeGameLegalMove *klondike_game_legal_move_new(KlondikeGameMoveType type, Stack *from, Stack *to);
void klondike_game_talon_move(KlondikeGame *kg);
void klondike_game_pile_move(KlondikeGame *kg);
void klondike_game_suite_moves(KlondikeGame *kg);
bool klondike_game_is_to_build_legal_move(const Card *from, Stack *to);

void klondike_game_legal_move_append(KlondikeGameLegalMove **head, KlondikeGameLegalMove *to_append);
KlondikeGame *klondike_game_new();
void klondike_game_build_moves(KlondikeGame *kg);
void klondike_game_move(KlondikeGameLegalMove *move);
void klondike_game_legal_move_destroy_all(KlondikeGameLegalMove **moves);
void klondike_game_save_state(KlondikeGame *kg, char *file_name);

bool klondike_game_init_random(void *game);
bool klondike_game_init_winning(void *game);
GameActionResult klondike_game_play_card(void *game);
GameActionResult klondike_game_iterate(void *game);
bool klondike_game_ended(void *game);
bool klondike_game_won(void *game);
void klondike_game_destroy(void *game);
bool klondike_game_reinit(void *game);
extern CardGame klondike_random;
extern CardGame klondike_winning;

#endif // KLONDIKE_H