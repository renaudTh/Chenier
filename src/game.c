#include "game.h"
#include "qll/qll.h"
#include "r7/r7.h"

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
bool card_game_play_graphic(GraphicContext *ctx, CardGame *cg) {
	cg->init(cg->game);
	graphic_context_render(ctx, cg->render, cg->game);
	GameActionResult res = {0};
	while (!cg->ended(cg->game)) {
		do {
			res = cg->play_card(cg->game);
			if (res.stateChanged) {
				graphic_context_wait_for_click();
				graphic_context_render(ctx, cg->render, cg->game);
			}
		} while (res.iterate);
		do {
			res = cg->iterate(cg->game);
			if (res.stateChanged) {
				graphic_context_wait_for_click();
				graphic_context_render(ctx, cg->render, cg->game);
			}
		} while (res.iterate);
	}
	return cg->won(cg->game);
}
CardGame card_game_new(CardGameType type) {
	CardGame g = {0};
	switch (type) {
		case CardGameTypeR7:
			g = r7_game;
			g.game = (R7Game *)r7_game_new();
			break;
		case CardGameTypeQLL:
			g = qll_game;
			g.game = (QllGame *)qll_game_new(52);
			break;
		default:
			break;
	}
	return g;
}

void card_game_destroy(CardGameType type) {
	switch (type) {
		case CardGameTypeR7:
			if (r7_game.game != NULL) {
				r7_game_destroy((R7Game *)r7_game.game);
			}
			break;
		case CardGameTypeQLL:
			if (qll_game.game != NULL) {
				qll_game_destroy((QllGame *)qll_game.game);
			}
			break;
		default:
			break;
	}
}