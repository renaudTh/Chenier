#include "game.h"
#include "c4/c4.h"
#include "klondike/klondike.h"
#include "qll/qll.h"
#include "r7/r7.h"
#include "up-down/up-down.h"

bool card_game_play(CardGame *cg) {

	GameActionResult res = {0};
	while (!cg->ended(cg->game)) {
		do {
			res = cg->play_card(cg->game);
		} while (res.iterate);
		do {
			res = cg->iterate(cg->game);
		} while (res.iterate);
	}
	return cg->won(cg->game);
}

void card_game_new(CardGame *game) {

	if (!game) return;

	switch (game->type) {
		case CardGameTypeR7:
			game->game = (R7Game *)r7_game_new();
			break;
		case CardGameTypeQLL:
			game->game = (QllGame *)qll_game_new(52);
			break;
		case CardGameTypeC4:
			game->game = (C4Game *)c4_game_new();
			break;
		case CardGameTypeUpDown:
			game->game = (UpDownGame *)up_down_game_new();
			break;
		case CardGameTypeKlondike:
			game->game = (KlondikeGame *)klondike_game_new();
			break;
		default:
			fprintf(stderr, "Unknown type of game");
			break;
	}
}
bool card_game_init(CardGame *game) {
	return game->init(game->game);
}
bool card_game_reinit(CardGame *game) {
	return game->reinit(game->game);
}
CardGame *card_game_copy(CardGame *game) {
	CardGame *ret = malloc(sizeof(CardGame));
	ret->ended = game->ended;
	ret->init = game->init;
	ret->iterate = game->iterate;
	ret->name = game->name;
	ret->play_card = game->play_card;
	ret->reinit = game->reinit;
	ret->type = game->type;
	ret->won = game->won;
	return ret;
}
void card_game_destroy(CardGame *game) {

	if (!game) return;

	switch (game->type) {
		case CardGameTypeR7:
			r7_game_destroy(game->game);
			break;
		case CardGameTypeQLL:
			qll_game_destroy(game->game);
			break;
		case CardGameTypeC4:
			c4_game_destroy(game->game);
			break;
		case CardGameTypeUpDown:
			up_down_game_destroy(game->game);
			break;
		case CardGameTypeKlondike:
			klondike_game_destroy(game->game);
			break;
		default:
			break;
	}
}