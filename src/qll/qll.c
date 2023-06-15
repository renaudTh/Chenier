#include "qll.h"

QllGame *qll_game_new(int nb_cards) {
	QllGame *qg = malloc(sizeof(QllGame));
	if (nb_cards != 32 && nb_cards != 52) {
		fprintf(stderr, "Please enter a valid number of cards (32 or 52)");
		return NULL;
	}
	qg->nb_card = nb_cards;
	qg->deck = stack_new_empty();
	qg->table = malloc(sizeof(Stack *) * nb_cards);
	qg->line_size = 0;
	return qg;
}
static void qll_push_stack_to_line(QllGame *qg) {
	qg->table[qg->line_size++] = stack_new_empty();
}
static Stack *qll_right_stack(QllGame *qg) {
	return qg->table[qg->line_size - 1];
}
static Stack *qll_middle_stack(QllGame *qg) {
	return qg->table[qg->line_size - 2];
}
static Stack *qll_left_stack(QllGame *qg) {
	return qg->table[qg->line_size - 3];
}

/* Winning game */
bool qll_game_winning_init(void *game) {
	QllGame *qg = (QllGame *)game;

	int color;
	for (int v = 14; v >= 7; v--) {
		color = 2 * (v % 2);
		for (int f = 0; f < 4; f++) {
			stack_add_card_on(qg->deck, card_new((color + f) % 4, v, false));
		}
	}
	for (int i = 0; i < 2; i++) {
		qll_push_stack_to_line(qg);
		stack_pop_from_top_to_top(qg->deck, qg->table[qg->line_size - 1], true);
	}
	return true;
}
/* (Loving you is a) Losing game*/
bool qll_game_losing_init(void *game) {
	QllGame *qg = (QllGame *)game;
	int new_f;
	unsigned j = 0;
	for (int f = 0; f < 4; f++) {
		for (int v = 14; v >= 7; v--) {
			new_f = ((f + v + (2 * j)) % 4);
			stack_add_card_on(qg->deck, card_new(new_f, v, false));
		}
		j++;
	}
	for (int i = 0; i < 2; i++) {
		qll_push_stack_to_line(qg);
		stack_pop_from_top_to_top(qg->deck, qg->table[qg->line_size - 1], true);
	}
	return true;
}
GameActionResult qll_game_play_card(void *game) {
	QllGame *qg = (QllGame *)game;
	GameActionResult res = {.iterate = false, .stateChanged = true};
	qll_push_stack_to_line(qg);
	stack_pop_from_top_to_top(qg->deck, qg->table[qg->line_size - 1], true);
	return res;
}
GameActionResult qll_game_iterate(void *game) {
	QllGame *qg = (QllGame *)game;
	GameActionResult res = {0};
	if (qg->line_size <= 2) {
		return res;
	}
	const Card *left = stack_read_first(qll_left_stack(qg));
	const Card *right = stack_read_first(qll_right_stack(qg));

	if ((card_value(left) == card_value(right)) || (card_family(left) == card_family(right))) {
		stack_append_stack_on_top(qll_left_stack(qg), qll_middle_stack(qg));
		stack_append_stack_on_top(qll_middle_stack(qg), qll_right_stack(qg));
		stack_destroy(qll_right_stack(qg));
		qg->line_size--;
		res.iterate = true;
		res.stateChanged = true;
		return res;
	}
	return res;
}
bool qll_game_ended(void *game) {
	QllGame *qg = (QllGame *)game;
	return stack_is_empty(qg->deck);
}
bool qll_game_won(void *game) {
	QllGame *qg = (QllGame *)game;
	return ((stack_is_empty(qg->deck)) && (qg->line_size == 2));
}

void qll_game_destroy(void *game) {
	QllGame *qg = (QllGame *)game;
	if (qg->deck) {
		stack_destroy(qg->deck);
	}
	if (qg->table) {
		while (qg->line_size > 0) {
			stack_destroy(qg->table[--qg->line_size]);
		}
		free(qg->table);
	}
	free(qg);
}

CardGame qll_win = {
    .ended = qll_game_ended,
    .init = qll_game_winning_init,
    .iterate = qll_game_iterate,
    .name = "The absolutely winning QLL",
    .play_card = qll_game_play_card,
    .won = qll_game_won,
    .type = CardGameTypeQLL,
};
CardGame qll_lose = {
    .ended = qll_game_ended,
    .init = qll_game_losing_init,
    .iterate = qll_game_iterate,
    .name = "The absolutely losing QLL",
    .play_card = qll_game_play_card,
    .won = qll_game_won,
    .type = CardGameTypeQLL,
};