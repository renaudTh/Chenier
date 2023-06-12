#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <r7/r7.h>

typedef struct GraphicContext {

	int width;
	int height;
	char *title;
	int card_width;
	int card_height;
	SDL_Renderer *screen;
	SDL_Texture *bg;
	SDL_Texture *sprite;

} GraphicContext;

typedef bool (*game_init)(void *);
typedef bool (*game_play_card)(void *);
typedef void (*game_iterate)(void *);
typedef bool (*game_ended)(void *);
typedef bool (*game_won)(void *);
typedef void (*game_render)(GraphicContext *, void *);

typedef struct CardGame {
	char *name;
	void *game;
	game_init init;
	game_play_card play_card;
	game_iterate iterate;
	game_ended ended;
	game_won won;
	game_render render;
} CardGame;

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
	SDL_Surface *background = IMG_Load("../assets/background.jpg");
	gc->bg = SDL_CreateTextureFromSurface(gc->screen, background);
	SDL_FreeSurface(background);

	SDL_Surface *sp = IMG_Load("../assets/spriteResized.png");
	gc->sprite = SDL_CreateTextureFromSurface(gc->screen, sp);
	SDL_FreeSurface(sp);

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

void graphic_context_wait(GraphicContext *gc, SDL_Event *event) {

	while (SDL_WaitEvent(event)) {
		switch (event->type) {
			case SDL_KEYDOWN:
			case SDL_MOUSEBUTTONDOWN:
				graphic_context_destroy(gc);
				exit(0);
			case SDL_QUIT:
				graphic_context_destroy(gc);
				exit(0);
			default:
				continue;
		}
	}
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

void graphic_context_render(GraphicContext *ctx, game_render render_function, void *user_data) {
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
void r7_game_render(GraphicContext *gc, void *game) {
	R7Game *rg = (R7Game *)game;
	graphic_context_plot_stack(gc, rg->deck, 0, 0, false);
	graphic_context_plot_stack(gc, rg->bin, gc->card_width + 2, 0, false);
	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(gc, rg->build[i], gc->card_width * (i + 1), gc->card_height + 2, true);
	}
}

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

int main() {

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_VideoInit(NULL);
	SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	GraphicContext *ctx = graphic_context_new(window, "Reussite R7", 1200, 900);
	CardGame g = {
	    .name = "Classical R7",
	    .game = (R7Game *)r7_game_new(),
	    .init = r7_init_winning_game_in_two_attempts,
	    .ended = r7_game_ending_condition,
	    .play_card = r7_game_play_card,
	    .won = r7_game_winning_condition,
	    .iterate = r7_game_iterate,
	    .render = r7_game_render,
	};
	bool win = card_game_play_graphic(ctx, &g);
	if (win) {
		printf("YOU WON\n");
	} else {
		printf("YOU LOSE\n");
	}
	r7_game_destroy((R7Game *)g.game);
	graphic_context_destroy(ctx);
	SDL_DestroyWindow(window);
	SDL_VideoQuit();
	SDL_Quit();
	IMG_Quit();
	return 0;
}
