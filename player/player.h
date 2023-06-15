#ifndef PLAYER_H
#define PLAYER_H

#include "graphic-context.h"

void r7_game_render(GraphicContext *gc, void *game);
void qll_game_render(GraphicContext *ctx, void *game);
void c4_game_render(GraphicContext *ctx, void *game);

#endif // PLAYER_H