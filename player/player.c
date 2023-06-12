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
	SDL_Window *window;
	SDL_Renderer *screen;
	SDL_Texture *bg;
	SDL_Texture *sprite;

} GraphicContext;

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
GraphicContext *graphic_context_new(char *title, int width, int height) {

	GraphicContext *gc = malloc(sizeof(GraphicContext));
	gc->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	gc->width = width;
	gc->height = height;

	gc->screen = SDL_CreateRenderer(gc->window, -1, 0);
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
	if (gc->window) {
		SDL_DestroyWindow(gc->window);
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

void r7_plot_table(GraphicContext *gc, R7Game *game) {
	graphic_context_plot_stack(gc, game->deck, 0, 0, false);
	graphic_context_plot_stack(gc, game->bin, gc->card_width + 2, 0, false);
	for (int i = 0; i < 4; i++) {
		graphic_context_plot_stack(gc, game->build[i], gc->card_width * (i + 1), gc->card_height + 2, true);
	}
}
int main() {

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_VideoInit(NULL);
	SDL_Event event;

	R7Game *rg = r7_game_new();
	r7_game_init(rg);
	GraphicContext *gc = graphic_context_new("Hello", 1200, 900);
	SDL_SetRenderDrawColor(gc->screen, 0, 0, 0, 255);
	SDL_RenderClear(gc->screen);
	graphic_context_blit_background(gc);
	r7_plot_table(gc, rg);
	SDL_RenderPresent(gc->screen);

	bool play = true;
	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				play = r7_game_play_card(rg);
				break;
			case SDL_MOUSEBUTTONDOWN:
				play = r7_game_play_card(rg);
				break;
			case SDL_QUIT:
				graphic_context_destroy(gc);
				exit(0);
			default:
				continue;
		}
		if (!play) {
			stack_flip(rg->bin);
			stack_append_stack_on_tail(rg->deck, rg->bin);
		}
		SDL_SetRenderDrawColor(gc->screen, 0, 0, 0, 255);
		SDL_RenderClear(gc->screen);
		graphic_context_blit_background(gc);
		r7_plot_table(gc, rg);
		SDL_RenderPresent(gc->screen);
	}
	r7_game_destroy(rg);
	graphic_context_destroy(gc);
	SDL_VideoQuit();
	SDL_Quit();
	IMG_Quit();
	return 0;
}
