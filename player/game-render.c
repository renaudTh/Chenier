#include "c4/c4.h"
#include "game.h"
#include "graphic-context.h"
#include "klondike/klondike.h"
#include "player.h"
#include "qll/qll.h"
#include "r7/r7.h"
#include "up-down/up-down.h"

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
void c4_game_render(GraphicContext *ctx, void *game) {
	C4Game *cg = (C4Game *)game;
	int x0 = (ctx->width - 4 * ctx->card_width) / 2;
	int y0 = (ctx->height - (8 * 0.16 + 1) * ctx->card_height) / 2;
	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(ctx, cg->build[i], x0, y0, true);
		x0 += ctx->card_width;
	}
}
void up_down_game_render(GraphicContext *ctx, void *game) {
	UpDownGame *udg = (UpDownGame *)game;

	graphic_context_plot_stack(ctx, udg->deck, 0, 0, false);

	int x = 2 * ctx->card_width;
	int y = 0;
	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(ctx, udg->stock[i], x, y, false);
		y += 1.2 * ctx->card_height;
		graphic_context_plot_stack(ctx, udg->build[i], x, y, true);
		x += ctx->card_width;
		y -= 1.2 * ctx->card_height;
	}
}

void klondike_game_render(GraphicContext *ctx, void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;

	graphic_context_plot_stack(ctx, kg->pile, 0, 0, false);
	graphic_context_plot_stack(ctx, kg->talon, ctx->card_width, 0, false);

	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(ctx, kg->suite[i], (i + 3) * ctx->card_width, 0, false);
	}
	for (int i = 0; i < 7; i++) {
		graphic_context_plot_stack(ctx, kg->build[i], i * ctx->card_width, ctx->card_height, true);
	}
}