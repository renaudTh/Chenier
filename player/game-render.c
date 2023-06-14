#include "game.h"
#include "graphic-context.h"
#include "player.h"
#include "qll/qll.h"
#include "r7/r7.h"

void qll_game_render(GraphicContext *ctx, void *game) {
	QllGame *qg = (QllGame *)game;
	graphic_context_plot_stack(ctx, qg->deck, 0, 0, false);
	int x = 0;
	int y = ctx->card_height;
	for (int i = 0; i < qg->line_size; i++) {
		graphic_context_plot_stack(ctx, qg->table[i], x, y, false);
		x += ctx->card_width;
		if (x > ctx->width) {
			x = 0;
			y += ctx->card_height;
		}
	}
}

void r7_game_render(GraphicContext *gc, void *game) {
	R7Game *rg = (R7Game *)game;
	graphic_context_plot_stack(gc, rg->deck, 0, 0, false);
	graphic_context_plot_stack(gc, rg->bin, gc->card_width + 2, 0, false);
	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(gc, rg->build[i], gc->card_width * (i + 1), gc->card_height + 2, true);
	}
}