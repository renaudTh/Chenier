#include "r7.h"

R7Game *r7_game_new() {
	R7Game *rg = malloc(sizeof(R7Game));
	rg->attempt_nb = 0;
	rg->deck = stack_new_empty();
	rg->bin = stack_new_empty();
	for (int i = 0; i < NB_R7_BUILD_STACK; i++) {
		rg->build[i] = stack_new_empty();
	}
	return rg;
}
bool r7_game_init(void *game) {
	R7Game *rg = (R7Game *)game;
	for (int f = 0; f < 4; f++) {
		for (int v = 2; v <= 14; v++) {
			Card *c = card_new(f, v, false);
			if (card_value(c) == 7) {
				card_flip(c);
				stack_add_card_on(rg->build[f], c);
			} else {
				stack_add_card_on(rg->deck, c);
			}
		}
	}
	stack_shuffle(rg->deck);
	return true;
}
void r7_init_without_deck(R7Game *rg) {
	for (int f = 0; f < 4; f++) {
		for (int v = 2; v <= 14; v++) {
			Card *c = card_new(f, v, false);
			if (card_value(c) == 7) {
				card_flip(c);
				stack_add_card_on(rg->build[f], c);
			} else {
				card_destroy(c);
			}
		}
	}
}

bool r7_init_winning_game_in_one_attempt(void *game) {
	R7Game *rg = (R7Game *)game;
	for (Family f = 0; f < 4; f++) {

		Card *c = card_new(f, 7, true);
		stack_add_card_on(rg->build[f], c);
		for (uint8_t v = 8; v <= 14; v++) {
			c = card_new(f, v, false);
			stack_add_card_under(rg->deck, c);
		}
		for (uint8_t v = 6; v >= 2; v--) {
			c = card_new(f, v, false);
			stack_add_card_under(rg->deck, c);
		}
	}
	return true;
}
bool r7_init_losing_game(void *game) {
	R7Game *rg = (R7Game *)game;
	Card *c = NULL;
	for (Family f = 0; f < 4; f++) {
		c = card_new(f, 7, true);
		stack_add_card_on(rg->build[f], c);
	}
	for (uint8_t v = 2; v <= 14; v++) {
		if (v == 7 || v == 6 || v == 8) continue;
		for (Family f = 0; f < 4; f++) {
			c = card_new(f, v, false);
			stack_add_card_under(rg->deck, c);
		}
	}
	for (Family f = 0; f < 4; f++) {
		c = card_new(f, 6, false);
		stack_add_card_under(rg->deck, c);
		c = card_new(f, 8, false);
		stack_add_card_under(rg->deck, c);
	}
	return true;
}
bool r7_init_winning_game_in_two_attempts(void *game) {

	R7Game *rg = (R7Game *)game;
	for (Family f = 0; f < 4; f++) {

		Card *c = card_new(f, 7, true);
		stack_add_card_on(rg->build[f], c);
		for (uint8_t v = 8; v <= 14; v++) {
			c = card_new(f, v, false);
			stack_add_card_under(rg->deck, c);
		}
		for (uint8_t v = 2; v <= 6; v++) {
			c = card_new(f, v, false);
			stack_add_card_under(rg->deck, c);
		}
	}
	return true;
}
GameActionResult r7_game_play_card(void *game) {

	R7Game *rg = (R7Game *)game;
	GameActionResult res = {0};

	if (stack_is_empty(rg->deck)) {
		res.iterate = false;
		res.stateChanged = false;
		return res;
	}
	const Card *card_to_play = stack_read_first(rg->deck);

	uint8_t card_to_play_value = card_value(card_to_play);
	Family build_index = card_family(card_to_play);

	const Card *max_card = stack_read_first(rg->build[build_index]);
	const Card *min_card = stack_read_bottom(rg->build[build_index]);

	uint8_t min_value = card_value(min_card);
	uint8_t max_value = card_value(max_card);

	if (card_to_play_value == max_value + 1) stack_pop_from_top_to_top(rg->deck, rg->build[build_index], true);
	else if (card_to_play_value == min_value - 1) stack_pop_from_top_to_bottom(rg->deck, rg->build[build_index], true);
	else stack_pop_from_top_to_top(rg->deck, rg->bin, true);
	res.iterate = true;
	res.stateChanged = true;
	return res;
}
bool r7_game_winning_condition(void *game) {
	R7Game *rg = (R7Game *)game;
	int s = 0;
	for (int i = 0; i < NB_R7_BUILD_STACK; i++) {
		s += stack_get_size(rg->build[i]);
	}
	return (s == 52 && rg->attempt_nb <= 3);
}
bool r7_game_ending_condition(void *game) {
	R7Game *rg = (R7Game *)game;
	return (r7_game_winning_condition(rg) || rg->attempt_nb >= 3);
}
GameActionResult r7_game_iterate(void *game) {
	R7Game *rg = (R7Game *)game;
	GameActionResult res = {0};
	rg->attempt_nb++;
	res.iterate = 0;
	if (stack_is_empty(rg->bin)) {
		res.stateChanged = 0;
		return res;
	}
	res.stateChanged = 1;
	stack_flip(rg->bin, true);
	stack_append_stack_on_bottom(rg->deck, rg->bin);
	return res;
}

void r7_game_destroy(R7Game *rg) {
	if (rg->deck) stack_destroy(rg->deck);
	if (rg->bin) stack_destroy(rg->bin);
	for (int i = 0; i < NB_R7_BUILD_STACK; i++) {
		if (rg->build[i]) stack_destroy(rg->build[i]);
	}
	free(rg);
}

bool r7_game_reinit(void *game) {
	R7Game *rg = (R7Game *)game;
	rg->attempt_nb = 0;
	stack_empty(rg->deck);
	stack_empty(rg->bin);
	for (int i = 0; i < NB_R7_BUILD_STACK; i++) {
		stack_empty(rg->build[i]);
	}
	return r7_game_init(game);
}

bool r7_game_main_loop(R7Game *rg) {

	GameActionResult res = {0};
	while (!r7_game_ending_condition(rg)) {
		do {
			res = r7_game_play_card(rg);
		} while (res.iterate);
		r7_game_iterate(rg);
	}
	return (rg->attempt_nb < 3);
}

CardGame r7_winning = {
    .ended = r7_game_ending_condition,
    .init = r7_init_winning_game_in_one_attempt,
    .iterate = r7_game_iterate,
    .name = "R7 game winning in one",
    .play_card = r7_game_play_card,
    .won = r7_game_winning_condition,
    .type = CardGameTypeR7,
    .reinit = r7_game_reinit,
};
CardGame r7_losing = {
    .ended = r7_game_ending_condition,
    .init = r7_init_losing_game,
    .iterate = r7_game_iterate,
    .name = "R7 game losing",
    .play_card = r7_game_play_card,
    .won = r7_game_winning_condition,
    .type = CardGameTypeR7,
    .reinit = r7_game_reinit,
};
CardGame r7_normal = {
    .ended = r7_game_ending_condition,
    .init = r7_game_init,
    .iterate = r7_game_iterate,
    .name = "R7 game normal",
    .play_card = r7_game_play_card,
    .won = r7_game_winning_condition,
    .type = CardGameTypeR7,
    .reinit = r7_game_reinit,
};