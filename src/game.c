#include "game.h"
#include "qll/qll.h"
#include "r7/r7.h"

bool card_game_play(CardGame *cg) {

	cg->init(cg->game);
	bool play = true;
	bool iter = true;
	while (!cg->ended(cg->game)) {
		do {
			play = cg->play_card(cg->game);
		} while (play);
		do {
			iter = cg->iterate(cg->game);
		} while (iter);
	}
	return cg->won(cg->game);
}
bool card_game_play_graphic(GraphicContext *ctx, CardGame *cg) {
	cg->init(cg->game);
	graphic_context_render(ctx, cg->render, cg->game);
	bool play = true;
	bool iter = false;
	while (!cg->ended(cg->game)) {
		do {
			graphic_context_wait_for_click();
			play = cg->play_card(cg->game);
			graphic_context_render(ctx, cg->render, cg->game);
		} while (play);

		do {
			graphic_context_wait_for_click();
			iter = cg->iterate(cg->game);
			graphic_context_render(ctx, cg->render, cg->game);

		} while (iter);
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