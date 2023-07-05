#include "player.h"
#include "parser.h"
#include <c4/c4.h>
#include <klondike/klondike.h>
#include <qll/qll.h>
#include <r7/r7.h>

#include <sys/time.h>
#include <up-down/up-down.h>
ChenierPlayer *chenier_player_new() {

	ChenierPlayer *player = malloc(sizeof(ChenierPlayer));
	player->enable_graphic = false;
	player->game_name = NULL;
	player->chosen_game = NULL;
	player->nb_games = 1;
	player->registry_size = 0;
	player->nb_threads = 1;
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
	} else if (strcmp(argv[*i], "--nb-games") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		player->nb_games = atoi(argv[*i]);
	} else if (strcmp(argv[*i], "--nb-threads") == 0) {
		CHECK_ARGS(*i, argc);
		*i += 1;
		player->nb_threads = atoi(argv[*i]);
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

void chenier_player_play_graphical_game(ChenierGame *game_chosen, int nb_games) {

	card_game_new(game_chosen->game);
	card_game_init(game_chosen->game);
	bool won = false;
	int played_count = 0;
	SDL_Window *win = chenier_player_init_graphics();
	GraphicContext *ctx = graphic_context_new(win, game_chosen->game->name, 800, 600);
	int won_games = 0;

	while (played_count < nb_games) {
		won = graphic_context_play_card_game(ctx, game_chosen->game, game_chosen->render);
		won_games += (won);
		played_count++;
		if (played_count < nb_games) card_game_reinit(game_chosen->game);
	}
	graphic_context_destroy(ctx);
	chenier_player_destroy_graphics(win);
	card_game_destroy(game_chosen->game);
}

int chenier_player_play_cli_game(ChenierGame *game_chosen, int nb_games) {

	CardGame *game = card_game_copy(game_chosen->game);
	card_game_new(game);
	card_game_init(game);
	bool won = false;
	int played_count = 0;

	int won_games = 0;
	while (played_count < nb_games) {
		won = card_game_play(game);
		won_games += (won);
		played_count++;
		if (played_count < nb_games) card_game_reinit(game);
	}
	card_game_destroy(game);
	free(game);
	return won_games;
}
void *chenier_play_in_thread(void *usr_data) {
	int *won = malloc(sizeof(int));
	ChenierPlayer *player = (ChenierPlayer *)usr_data;
	int nb_games = player->nb_games / player->nb_threads;
	*won = chenier_player_play_cli_game(player->chosen_game, nb_games);
	return (void *)won;
}

int chenier_player_play_multithread(ChenierPlayer *player) {
	pthread_t *threads = malloc(sizeof(pthread_t) * player->nb_threads);
	int nb_win = 0;
	for (int i = 0; i < player->nb_threads; i++) {
		pthread_create(&threads[i], NULL, chenier_play_in_thread, player);
	}
	for (int i = 0; i < player->nb_threads; i++) {
		void *ret = NULL;
		pthread_join(threads[i], &ret);
		nb_win += *(int *)ret;
		free(ret);
	}
	free(threads);
	return nb_win;
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
	chenier_player_register_game(player, &up_down_winning, up_down_game_render);
	chenier_player_register_game(player, &klondike_random, klondike_game_render);
	chenier_player_register_game(player, &klondike_winning, klondike_game_render);

	ChenierParser parser = {.argc = argc, .argv = argv, .parse = chenier_player_parse_arg, .user_data = player};
	ChenierParseStatus res = chenier_parser_parse(&parser);
	if (res != ChenierParseStatus_PARSING_OK) exit(1);

	player->chosen_game = chenier_player_find_game_by_name(player);
	if (!player->chosen_game) {
		printf("Game not found. ");
		chenier_player_list_registered_games(player);
		exit(0);
	}
	if (player->enable_graphic) {
		if (player->nb_threads > 1) {
			fprintf(stderr, "Cannot play graphical game in multithread. Number of threads set to 1\n");
			player->nb_threads = 1;
		}
		chenier_player_play_graphical_game(player->chosen_game, player->nb_games);
	} else {
		int nb_win = 0;
		float winrate = 0.0;
		double exec_time;

		struct timeval begin, end;
		gettimeofday(&begin, 0);
		if (player->nb_threads > 1 && player->nb_games > 1) {
			nb_win = chenier_player_play_multithread(player);
		} else {
			nb_win = chenier_player_play_cli_game(player->chosen_game, player->nb_games);
		}
		gettimeofday(&end, 0);
		exec_time = (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec) * 1e-6;
		winrate = (float)nb_win / (float)player->nb_games;
		printf("========== Game Execution report ==========\n");
		printf("Game : %s\n", player->game_name);
		printf("Number of games : %d\n", player->nb_games);
		printf("Threads : %d\n", player->nb_threads);
		printf("Winrate : %f\n", winrate);
		printf("Execution time : %.3f s\n", exec_time);
		printf("===========================================\n");
	}

	chenier_player_destroy(player);
	return 0;
}
