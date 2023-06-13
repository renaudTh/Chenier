#include "game.h"

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
			iter = cg->iterate(cg->game);
			graphic_context_render(ctx, cg->render, cg->game);
			if (iter) graphic_context_wait_for_click();
		} while (iter);
	}
	return cg->won(cg->game);
}