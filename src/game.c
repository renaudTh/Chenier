#include "game.h"

bool card_game_play(CardGame *cg) {

	cg->init(cg->game);
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
