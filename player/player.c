#include "player.h"
#include "parser.h"
#include <c4/c4.h>
#include <qll/qll.h>
#include <r7/r7.h>

ChenierPlayer *chenier_player_new() {

	ChenierPlayer *player = malloc(sizeof(ChenierPlayer));
	player->enable_graphic = false;
	player->game_name = NULL;
	player->replay = 0;
	player->registry_size = 0;

	return player;
}
ChenierParseStatus chenier_player_parse_arg(int argc, char **argv, int *i, void *user_data) {

	ChenierPlayer *player = (ChenierPlayer *)user_data;
	if (strcmp(argv[*i], "--game") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		player->game_name = argv[*i];
	} else if (strcmp(argv[*i], "--enable-graphic") == 0) {
		player->enable_graphic = true;
	} else if (strcmp(argv[*i], "--replay") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		player->replay = atoi(argv[*i]);
	} else {
		return ChenierParseStatus_UNKNOWN_COMMAND;
	}
	return ChenierParseStatus_PARSING_OK;
}

void chenier_player_register_game(ChenierPlayer *p, CardGame *game, render_fct render) {
	ChenierGame *g = malloc(sizeof(ChenierGame));
	g->game = game;
	g->render = render;
	p->game_registry[p->registry_size++] = g;
}

void chenier_player_list_registered_games(ChenierPlayer *p) {
	printf("List of registered games :\n");
	for (int i = 0; i < p->registry_size; i++) {
		char *current_name = p->game_registry[i]->game->name;
		printf("%s\n", current_name);
	}
}
ChenierGame *chenier_player_find_game_by_name(ChenierPlayer *p) {

	if (!p->game_name) return NULL;
	for (int i = 0; i < p->registry_size; i++) {
		char *current_name = p->game_registry[i]->game->name;
		if (strcmp(current_name, p->game_name) == 0) {
			return p->game_registry[i];
		}
	}
	return NULL;
}

SDL_Window *chenier_player_init_graphics() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	SDL_Window *win = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	return win;
}
void chenier_player_destroy_graphics(SDL_Window *win) {
	SDL_DestroyWindow(win);
	SDL_Quit();
	IMG_Quit();
}
void chenier_player_destroy(ChenierPlayer *p) {
	if (!p) return;
	for (int i = 0; i < p->registry_size; i++) {
		if (p->game_registry[i]) {
			free(p->game_registry[i]);
		}
	}
	free(p);
}
int main(int argc, char **argv) {
	srand(time(NULL));
	ChenierPlayer *player = chenier_player_new();
	chenier_player_register_game(player, &r7_winning, r7_game_render);
	chenier_player_register_game(player, &r7_losing, r7_game_render);
	chenier_player_register_game(player, &r7_normal, r7_game_render);
	chenier_player_register_game(player, &qll_win, qll_game_render);
	chenier_player_register_game(player, &qll_lose, qll_game_render);
	chenier_player_register_game(player, &c4_normal, c4_game_render);

	ChenierParser parser = {.argc = argc, .argv = argv, .parse = chenier_player_parse_arg, .user_data = player};
	ChenierParseStatus res = chenier_parser_parse(&parser);
	if (res != ChenierParseStatus_PARSING_OK) exit(1);

	ChenierGame *chosen = chenier_player_find_game_by_name(player);
	if (!chosen) {
		printf("Game not found. ");
		chenier_player_list_registered_games(player);
		exit(0);
	}
	card_game_new(chosen->game);
	bool won = false;
	if (player->enable_graphic) {
		SDL_Window *win = chenier_player_init_graphics();
		GraphicContext *ctx = graphic_context_new(win, chosen->game->name, 1200, 900);
		won = graphic_context_play_card_game(ctx, chosen->game, chosen->render);
		graphic_context_destroy(ctx);
		chenier_player_destroy_graphics(win);
	} else {
		won = card_game_play(chosen->game);
	}
	if (won) printf("YOU WON!\n");
	else printf("YOU LOSE!\n");
	card_game_destroy(chosen->game);
	chenier_player_destroy(player);
	return 0;
}
