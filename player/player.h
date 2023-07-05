#ifndef PLAYER_H
#define PLAYER_H
#define MAX_GAME_PLAYER 100
#include "graphic-context.h"
typedef struct ChenierGame {
	CardGame *game;
	render_fct render;
} ChenierGame;

typedef struct ChenierPlayer {

	ChenierGame *game_registry[MAX_GAME_PLAYER];
	int registry_size;
	char *game_name;
	int nb_games;
	bool enable_graphic;
} ChenierPlayer;

void r7_game_render(GraphicContext *gc, void *game);
void qll_game_render(GraphicContext *ctx, void *game);
void c4_game_render(GraphicContext *ctx, void *game);
void up_down_game_render(GraphicContext *ctx, void *game);
void klondike_game_render(GraphicContext *ctx, void *game);
#endif // PLAYER_H