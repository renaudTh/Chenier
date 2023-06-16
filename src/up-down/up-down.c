#include "up-down.h"

UpDownGame *up_down_game_new() {
	UpDownGame *udg = malloc(sizeof(UpDownGame));
	for (int i = 0; i < 4; i++) {
		udg->build[i] = stack_new_empty();
		udg->stock[i] = stack_new_empty();
	}
	udg->deck = stack_new_empty();
	udg->end = false;
	return udg;
}

bool up_down_game_init(void *game) {
	UpDownGame *udg = (UpDownGame *)game;
	udg->deck = stack_create_complete_deck(32, false);
	stack_shuffle(udg->deck);
	return true;
}
bool up_down_game_winning_init(void *game) {
	UpDownGame *udg = (UpDownGame *)game;

	for (int v = 8; v <= 14; v++) {
		for (int f = 0; f < 4; f++) {
			stack_add_card_on(udg->deck, card_new(f, v, false));
		}
	}
	for (int f = 0; f < 4; f++) {
		stack_add_card_on(udg->deck, card_new(f, 7, false));
	}
	return true;
}
static int up_down_game_chose_naive_stockage(UpDownGame *udg, const Card *current_card) {

	uint8_t current_value = card_value(current_card);

	for (int i = 0; i < 4; i++) {
		const Card *top = stack_read_first(udg->stock[i]);
		if (!top) return i;
		if (current_value == card_value(top) - 1) return i;
	}
	return -1;
}

static bool up_down_game_try_build(UpDownGame *udg, Stack *from) {
	const Card *card = stack_read_first(from);
	if (!card) {
		return false;
	}
	Family f = card_family(card);
	uint8_t v = card_value(card);
	bool visible = card_is_visible(card);
	const Card *top = stack_read_first(udg->build[f]);
	if (!top && v == 7) {
		stack_pop_from_top_to_top(from, udg->build[f], !visible);
		return true;
	} else if (top && (v == card_value(top) + 1)) {
		stack_pop_from_top_to_top(from, udg->build[f], !visible);
		return true;
	}
	return false;
}
static bool up_down_game_try_stock(UpDownGame *udg, Stack *from) {
	const Card *card = stack_read_first(from);

	int stock_id = up_down_game_chose_naive_stockage(udg, card);
	if (stock_id != -1) {
		stack_pop_from_top_to_top(from, udg->stock[stock_id], true);
		return true;
	}
	return false;
}
GameActionResult up_down_game_play_card(void *game) {
	UpDownGame *udg = (UpDownGame *)game;
	GameActionResult ret = {0};

	bool build = up_down_game_try_build(udg, udg->deck);
	if (build) {
		ret.stateChanged = true;
		return ret;
	}
	bool stock = up_down_game_try_stock(udg, udg->deck);
	if (stock) {
		ret.stateChanged = true;
		return ret;
	}
	ret.stateChanged = false;
	udg->end = true;
	return ret;
}
GameActionResult up_down_game_iterate(void *game) {
	UpDownGame *udg = (UpDownGame *)game;
	GameActionResult ret = {0};
	for (int i = 0; i < 4; i++) {
		bool build = up_down_game_try_build(udg, udg->stock[i]);
		if (build) {
			ret.iterate = true;
			ret.stateChanged = true;
			return ret;
		}
	}
	return ret;
}
bool up_down_game_winning_condition(void *game) {
	UpDownGame *udg = (UpDownGame *)game;

	bool win = stack_is_empty(udg->deck);
	for (int i = 0; i < 4; i++) {
		win &= stack_is_empty(udg->stock[i]);
	}
	return win;
}
bool up_down_game_ending_condition(void *game) {
	UpDownGame *udg = (UpDownGame *)game;
	return (udg->end || up_down_game_winning_condition(udg));
}

void up_down_game_destroy(UpDownGame *udg) {
	for (int i = 0; i < 4; i++) {
		if (udg->build[i]) stack_destroy(udg->build[i]);
		if (udg->stock[i]) stack_destroy(udg->stock[i]);
	}
	if (udg->deck) stack_destroy(udg->deck);
	free(udg);
}

CardGame up_down_winning = {
    .ended = up_down_game_ending_condition,
    .init = up_down_game_winning_init,
    .iterate = up_down_game_iterate,
    .name = "Up down winning",
    .play_card = up_down_game_play_card,
    .type = CardGameTypeUpDown,
    .won = up_down_game_winning_condition,
};