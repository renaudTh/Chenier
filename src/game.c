#include "game.h"

bool card_game_play(CardGame *cg) {

	cg->init(cg->game);
	bool play = true;
	while (!cg->ended(cg->game)) {
		do {
			play = cg->play_card(cg->game);
		} while (play);
		cg->iterate(cg->game);
	}
	return cg->won(cg->game);
}
bool card_game_play_graphic(GraphicContext *ctx, CardGame *cg) {
	cg->init(cg->game);
	graphic_context_render(ctx, cg->render, cg->game);
	bool play = true;
	while (!cg->ended(cg->game)) {
		do {
			graphic_context_wait_for_click();
			play = cg->play_card(cg->game);
			graphic_context_render(ctx, cg->render, cg->game);
		} while (play);
		cg->iterate(cg->game);
		graphic_context_render(ctx, cg->render, cg->game);
	}
	return cg->won(cg->game);
}