#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "graphic-context.h"
#include <qll/qll.h>

int main() {

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_VideoInit(NULL);
	SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

	CardGame g = qll_game;
	g.game = (QllGame *)qll_game_new(52);

	GraphicContext *ctx = graphic_context_new(window, g.name, 800, 600);

	bool win = card_game_play_graphic(ctx, &g);
	if (win) {
		printf("YOU WON\n");
	} else {
		printf("YOU LOSE\n");
	}
	qll_game_destroy((QllGame *)g.game);
	graphic_context_destroy(ctx);
	SDL_DestroyWindow(window);
	SDL_VideoQuit();
	SDL_Quit();
	IMG_Quit();
	return 0;
}
