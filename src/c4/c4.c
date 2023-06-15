#include "c4.h"

C4Game *c4_game_new() {

	C4Game *cg = malloc(sizeof(C4Game));
	cg->deck = stack_create_complete_deck(32, false);
	for (int i = 0; i < 4; i++) {
		cg->build[i] = stack_new_empty();
	}
	cg->is_ended = false;
	cg->is_win = false;
	cg->last_build_played = -1;
	return cg;
}
bool c4_game_init(void *game) {
	C4Game *cg = (C4Game *)game;
	stack_shuffle(cg->deck);
	int i = 0;
	while (stack_get_size(cg->deck) > 0) {
		stack_pop_from_top_to_top(cg->deck, cg->build[i % 4], false);
		i++;
	}
	return true;
}

GameActionResult c4_game_play_card(void *game) {
	C4Game *cg = (C4Game *)game;
	GameActionResult res = {.iterate = true, .stateChanged = true};
	if (cg->last_build_played == -1) {
		cg->last_build_played = rand() % 4;
	}
	const Card *c = stack_read_first(cg->build[cg->last_build_played]);
	uint8_t family = card_family(c);
	if (cg->last_build_played == family) {
	}

	if (card_is_visible(stack_read_first(cg->build[family]))) {
		res.iterate = false;
	}
	stack_pop_from_top_to_bottom(cg->build[cg->last_build_played], cg->build[family], true);
	cg->last_build_played = family;
	return res;
}
bool c4_game_winning_condition(void *game) {
	C4Game *cg = (C4Game *)game;
	return cg->is_win;
}
bool c4_game_ending_condition(void *game) {
	C4Game *cg = (C4Game *)game;
	return cg->is_ended;
}
GameActionResult c4_game_iterate(void *game) {
	C4Game *cg = (C4Game *)game;
	cg->is_ended = true;
	cg->is_win = true;
	Node *runner = NULL;
	for (int family = 0; family < 4; family++) {
		runner = cg->build[family]->head;
		while (runner != NULL && !card_is_visible(runner->card)) {
			card_flip(runner->card);
			if ((int)card_family(runner->card) != family) {
				cg->is_win = false;
			}
			runner = runner->next;
		}
	}
	GameActionResult res = {.iterate = false, .stateChanged = true};
	return res;
}
void c4_game_destroy(C4Game *cg) {
	if (cg->deck) {
		stack_destroy(cg->deck);
	}
	for (int i = 0; i < 4; i++) {
		if (cg->build[i]) {
			stack_destroy(cg->build[i]);
		}
	}
	free(cg);
}
CardGame c4_normal = {
    .ended = c4_game_ending_condition,
    .init = c4_game_init,
    .iterate = c4_game_iterate,
    .name = "C4 Game",
    .play_card = c4_game_play_card,
    .won = c4_game_winning_condition,
    .type = CardGameTypeC4,
};