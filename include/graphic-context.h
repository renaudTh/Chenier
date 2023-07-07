#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "stack.h"

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

typedef void (*render_fct)(GraphicContext *, void *);

GraphicContext *graphic_context_new(SDL_Window *window, char *title, int width, int height);
void graphic_context_destroy(GraphicContext *gc);
SDL_Rect graphic_context_get_card_rect(GraphicContext *gc, const Card *card);
void graphic_context_plot_card(GraphicContext *gc, const Card *card, int x, int y);
void graphic_context_plot_stack(GraphicContext *gc, Stack *stack, int x, int y, bool spread);
void graphic_context_render(GraphicContext *ctx, render_fct render_function, void *user_data);
void graphic_context_wait_for_click();

bool graphic_context_play_card_game(GraphicContext *ctx, CardGame *cg, render_fct game_render);

#endif // GRAPHIC_CONTEXT_H