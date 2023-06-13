#include "qll.h"
#include "game.h"

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
bool qll_game_init(void *game) {
	QllGame *qg = (QllGame *)game;
	for (int f = 0; f < 4; f++) {
		for (int v = 14; v >= 7; v--) {
			stack_add_card_on(qg->deck, card_new(f, v, false));
		}
	}
	for (int i = 0; i < 2; i++) {
		qll_push_stack_to_line(qg);
		stack_pop_from_top_to_top(qg->deck, qg->table[qg->line_size - 1], true);
	}
	return true;
}
bool qll_game_play_card(void *game) {
	QllGame *qg = (QllGame *)game;
	qll_push_stack_to_line(qg);
	stack_pop_from_top_to_top(qg->deck, qg->table[qg->line_size - 1], true);
	return false;
}
bool qll_game_iterate(void *game) {
	QllGame *qg = (QllGame *)game;
	if (qg->line_size <= 2) return false;
	const Card *left = stack_read_first(qll_left_stack(qg));
	const Card *right = stack_read_first(qll_right_stack(qg));

	if ((card_value(left) == card_value(right)) || (card_family(left) == card_family(right))) {
		stack_append_stack_on_top(qll_left_stack(qg), qll_middle_stack(qg));
		stack_append_stack_on_top(qll_middle_stack(qg), qll_right_stack(qg));
		stack_destroy(qll_right_stack(qg));
		qg->line_size--;
		return true;
	}
	return false;
}
bool qll_game_ended(void *game) {
	QllGame *qg = (QllGame *)game;
	return stack_is_empty(qg->deck);
}
bool qll_game_won(void *game) {
	QllGame *qg = (QllGame *)game;
	return ((stack_is_empty(qg->deck)) && (qg->line_size == 2));
}
void qll_game_render(GraphicContext *ctx, void *game) {
	QllGame *qg = (QllGame *)game;
	graphic_context_plot_stack(ctx, qg->deck, 0, 0, false);
	int x = 0;
	int y = ctx->card_height;
	for (int i = 0; i < qg->line_size; i++) {
		graphic_context_plot_stack(ctx, qg->table[i], x, y, false);
		x += ctx->card_width;
		if (x > ctx->width) {
			x = 0;
			y += ctx->card_height;
		}
	}
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
CardGame qll_game = {
    .name = "The QLL",
    .init = qll_game_init,
    .ended = qll_game_ended,
    .play_card = qll_game_play_card,
    .won = qll_game_won,
    .iterate = qll_game_iterate,
    .render = qll_game_render,
};