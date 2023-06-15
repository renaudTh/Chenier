#include "graphic-context.h"
#include "game.h"

static void graphic_context_blit_background(GraphicContext *gc) {
	SDL_Rect dst = {0}, src = {0};
	int bgw, bgh;

	SDL_QueryTexture(gc->bg, NULL, NULL, &bgw, &bgh);
	src.x = (bgw - gc->width) / 2;
	src.y = (bgh - gc->height) / 2;
	src.w = gc->width;
	src.h = gc->height;
	dst = (SDL_Rect){.x = 0, .y = 0, .w = gc->width, .h = gc->height};
	SDL_RenderCopy(gc->screen, gc->bg, &src, &dst);
}
GraphicContext *graphic_context_new(SDL_Window *window, char *title, int width, int height) {

	GraphicContext *gc = malloc(sizeof(GraphicContext));
	SDL_SetWindowTitle(window, title);
	SDL_SetWindowSize(window, width, height);
	gc->width = width;
	gc->height = height;

	gc->screen = SDL_CreateRenderer(window, -1, 0);
	char *basepath = SDL_GetBasePath();
	char backPath[150];
	strcpy(backPath, basepath);
	strcat(backPath, "assets/background.jpg");
	gc->bg = IMG_LoadTexture(gc->screen, backPath);
	if (!gc->bg) {
		fprintf(stderr, "%s", IMG_GetError());
	}
	char spritePath[150];
	strcpy(spritePath, basepath);
	strcat(spritePath, "assets/spriteResized.png");
	gc->sprite = IMG_LoadTexture(gc->screen, spritePath);

	SDL_QueryTexture(gc->sprite, NULL, NULL, &gc->card_width, &gc->card_height);
	gc->card_height /= 5;
	gc->card_width /= 13;
	graphic_context_blit_background(gc);
	return gc;
}

void graphic_context_destroy(GraphicContext *gc) {
	if (gc->bg) {
		SDL_DestroyTexture(gc->bg);
	}
	if (gc->sprite) {
		SDL_DestroyTexture(gc->sprite);
	}
	if (gc->screen) {
		SDL_DestroyRenderer(gc->screen);
	}
	free(gc);
}

SDL_Rect graphic_context_get_card_rect(GraphicContext *gc, const Card *card) {
	SDL_Rect ret = {0};
	ret.w = gc->card_width;
	ret.h = gc->card_height;
	if (card_is_visible(card)) {
		ret.x = (card_value(card) - 2) * gc->card_width;
		ret.y = (card_family(card)) * gc->card_height;
	} else {
		ret.x = 2 * gc->card_width;
		ret.y = 4 * gc->card_height;
	}
	return ret;
}
void graphic_context_plot_card(GraphicContext *gc, const Card *card, int x, int y) {

	if (!card) return;
	SDL_Rect src = graphic_context_get_card_rect(gc, card);
	SDL_Rect dst = {.x = x, .y = y, .w = gc->card_width, .h = gc->card_height};
	SDL_RenderCopy(gc->screen, gc->sprite, &src, &dst);
}

void graphic_context_plot_stack(GraphicContext *gc, Stack *stack, int x, int y, bool spread) {

	if (!spread) {
		graphic_context_plot_card(gc, stack_read_first(stack), x, y);
	} else {
		// TODO : implement a stack iterator
		int xi = x;
		int yi = y;
		Node *runner = stack->tail;
		while (runner != NULL) {
			SDL_Rect src = graphic_context_get_card_rect(gc, runner->card);
			SDL_Rect dst = {.x = xi, .y = yi, .w = gc->card_width, .h = gc->card_height};
			SDL_RenderCopy(gc->screen, gc->sprite, &src, &dst);
			runner = runner->prev;
			yi += gc->card_height * 0.16;
		}
	}
}
void graphic_context_render(GraphicContext *ctx, render_fct render_function, void *user_data) {
	SDL_SetRenderDrawColor(ctx->screen, 0, 0, 0, 255);
	SDL_RenderClear(ctx->screen);
	graphic_context_blit_background(ctx);
	render_function(ctx, user_data);
	SDL_RenderPresent(ctx->screen);
}
void graphic_context_wait_for_click() {
	SDL_Event event;
	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
			case SDL_MOUSEBUTTONDOWN:
				return;
			case SDL_QUIT:
				exit(0);
			default:
				continue;
		}
	}
}
bool graphic_context_play_card_game(GraphicContext *ctx, CardGame *cg, render_fct game_render) {
	cg->init(cg->game);
	graphic_context_render(ctx, game_render, cg->game);
	GameActionResult res = {0};
	while (!cg->ended(cg->game)) {
		do {
			res = cg->play_card(cg->game);
			if (res.stateChanged) {
				graphic_context_wait_for_click();
				graphic_context_render(ctx, game_render, cg->game);
			}
		} while (res.iterate);
		do {
			res = cg->iterate(cg->game);
			if (res.stateChanged) {
				graphic_context_wait_for_click();
				graphic_context_render(ctx, game_render, cg->game);
			}
		} while (res.iterate);
	}
	graphic_context_wait_for_click();
	return cg->won(cg->game);
}