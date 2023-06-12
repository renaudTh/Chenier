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
void r7_game_init(R7Game *rg) {
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
}
bool r7_game_play_card(R7Game *rg) {

	if (stack_is_empty(rg->deck)) return false;
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

	return true;
}
bool r7_game_main_loop(R7Game *rg) {
	bool play = true;
	while (rg->attempt_nb < 3) {
		do {
			play = r7_game_play_card(rg);
		} while (play);
		rg->attempt_nb++;
		if (stack_is_empty(rg->bin)) break;
		else stack_append_stack_on_tail(rg->deck, rg->bin);
	}
	return (rg->attempt_nb < 3);
}
void r7_game_destroy(R7Game *rg) {
	if (rg->deck) stack_destroy(rg->deck);
	if (rg->bin) stack_destroy(rg->bin);
	for (int i = 0; i < NB_R7_BUILD_STACK; i++) {
		if (rg->build[i]) stack_destroy(rg->build[i]);
	}
	free(rg);
}