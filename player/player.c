#include "player.h"

#include <c4/c4.h>
#include <qll/qll.h>
#include <r7/r7.h>

int main() {
	srand(time(NULL));
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	SDL_VideoInit(NULL);
	SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

	// CardGame qll = {
	//     .init = qll_game_winning_init,
	//     .play_card = qll_game_play_card,
	//     .iterate = qll_game_iterate,
	//     .won = qll_game_won,
	//     .ended = qll_game_ended,
	//     .name = "The Winning QLL",
	// };

	// CardGame r7 = {
	//     .init = r7_init_winning_game_in_one_attempt,
	//     .play_card = r7_game_play_card,
	//     .iterate = r7_game_iterate,
	//     .won = r7_game_winning_condition,
	//     .ended = r7_game_ending_condition,
	//     .name = "The Winning R7",
	// };

	CardGame c4 = {
	    .init = c4_game_init,
	    .play_card = c4_game_play_card,
	    .iterate = c4_game_iterate,
	    .won = c4_game_winning_condition,
	    .ended = c4_game_ending_condition,
	    .name = "The C4",
	};
	CardGame g = c4;
	g.game = (C4Game *)c4_game_new();

	GraphicContext *ctx = graphic_context_new(window, g.name, 1200, 600);

	bool win = graphic_context_play_card_game(ctx, &g, c4_game_render);
	if (win) {
		printf("YOU WON\n");
	} else {
		printf("YOU LOSE\n");
	}
	c4_game_destroy((C4Game *)g.game);
	graphic_context_destroy(ctx);
	SDL_DestroyWindow(window);
	SDL_VideoQuit();
	SDL_Quit();
	IMG_Quit();
	return 0;
}
