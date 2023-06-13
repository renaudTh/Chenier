#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "graphic-context.h"
#include <r7/r7.h>

int main() {

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_VideoInit(NULL);
	SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

	CardGame g = r7_game;
	g.game = (R7Game *)r7_game_new();

	GraphicContext *ctx = graphic_context_new(window, g.name, 1200, 900);

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
