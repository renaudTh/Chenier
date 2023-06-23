#include "klondike.h"

KlondikeGame *klondike_game_new() {

	KlondikeGame *kg = malloc(sizeof(KlondikeGame));
	kg->pile = stack_new_empty();
	kg->talon = stack_new_empty();
	for (int i = 0; i < 4; i++) {
		kg->suite[i] = stack_new_empty();
	}
	for (int i = 0; i < 7; i++) {
		kg->build[i] = stack_new_empty();
	}
	return kg;
}

void klondike_game_destroy(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;

	if (kg->pile) stack_destroy(kg->pile);
	if (kg->talon) stack_destroy(kg->talon);
	for (int i = 0; i < 4; i++) {
		if (kg->suite[i]) stack_destroy(kg->suite[i]);
	}
	for (int i = 0; i < 7; i++) {
		if (kg->build[i]) stack_destroy(kg->build[i]);
	}
	free(kg);
}

bool klondike_game_is_to_build_legal_move(const Card *from, Stack *to) {

	if (stack_is_empty(to)) {
		return (card_value(from) == 13);
	}
	const Card *to_card = stack_read_first(to);

	return (card_value(to_card) == card_value(from) + 1) &&
	       (((card_family(from) % 2) & (card_family(to_card) % 2)) == 0);
}

bool klondike_game_is_to_suite_legal_move(KlondikeGame *kg, const Card *from) {

	Family f = card_family(from);
	if (stack_is_empty(kg->suite[f])) {
		return (card_value(from) == 1);
	}
	const Card *to_card = stack_read_first(kg->suite[f]);
	return ((card_value(to_card)) == (card_value(from) - 1));
}
KlondikeGameLegalMove *klondike_game_legal_move_new(KlondikeGameMoveType type, Stack *from, Stack *to) {
	KlondikeGameLegalMove *m = malloc(sizeof(KlondikeGameLegalMove));
	m->from = from;
	m->to = to;
	m->type = type;
	m->next = NULL;
	m->index_from = 0;
	return m;
}
void klondike_game_legal_move_append(KlondikeGameLegalMove **head, KlondikeGameLegalMove *to_append) {

	if (to_append == NULL) return;
	if (*head == NULL) {
		*head = to_append;
		return;
	}
	KlondikeGameLegalMove *runner = *head;
	while (runner->next != NULL) {
		runner = runner->next;
	}
	runner->next = to_append;
}
KlondikeGameLegalMove *klondike_game_pile_move(KlondikeGame *kg) {

	KlondikeGameLegalMove *m = NULL;
	if (stack_is_empty(kg->pile)) {
		m = klondike_game_legal_move_new(Redeal, kg->talon, kg->pile);
	} else {
		m = klondike_game_legal_move_new(PileToTalon, kg->pile, kg->talon);
	}
	return m;
}

KlondikeGameLegalMove *klondike_game_talon_move(KlondikeGame *kg) {

	if (kg->talon == NULL) return NULL;
	const Card *c = stack_read_first(kg->talon);
	// Talon to build
	KlondikeGameLegalMove *head = NULL, *move = NULL;
	for (int i = 0; i < 7; i++) {
		if (!klondike_game_is_to_build_legal_move(c, kg->build[i])) continue;
		move = klondike_game_legal_move_new(TalonToBuild, kg->talon, kg->build[i]);
		klondike_game_legal_move_append(&head, move);
	}
	// Talon to suite
	if (klondike_game_is_to_suite_legal_move(kg, c)) {
		move = klondike_game_legal_move_new(TalonToSuite, kg->talon, kg->suite[card_family(c)]);
		klondike_game_legal_move_append(&head, move);
	}
	return head;
}

KlondikeGameLegalMove *klondike_game_build_to_build(KlondikeGame *kg, Stack *from) {

	if (!from || stack_is_empty(from)) return NULL;
	StackIterator *it = stack_begin(from);
	KlondikeGameLegalMove *head = NULL, *move = NULL;
	int index = 0;
	while (card_is_visible(it->card) && it != stack_end(from)) {
		for (int i = 0; i < 7; i++) {
			if (from == kg->build[i]) continue;
			if (!klondike_game_is_to_build_legal_move(it->card, kg->build[i])) continue;
			move = klondike_game_legal_move_new(BuildToBuild, from, kg->build[i]);
			move->index_from = index;
			klondike_game_legal_move_append(&head, move);
		}
		it = stack_next(it);
		index++;
	}
	return head;
}

KlondikeGameLegalMove *klondike_game_build_moves(KlondikeGame *kg) {

	KlondikeGameLegalMove *moves = NULL, *current = NULL;
	for (int i = 0; i < 7; i++) {
		current = klondike_game_build_to_build(kg, kg->build[i]);
		klondike_game_legal_move_append(&moves, current);
	}
	return moves;
}

void Klondike_game_move(KlondikeGameLegalMove *move) {

	Stack *temp = stack_split(move->from, move->index_from);
	stack_append_stack_on_top(move->to, temp);
	stack_destroy(temp);
}

KlondikeGameLegalMove *klondike_game_suite_moves(KlondikeGame *kg) {
	KlondikeGameLegalMove *moves = NULL, *current = NULL;
	for (int i = 0; i < 7; i++) {
		if (stack_is_empty(kg->build[i])) continue;
		const Card *card = stack_read_first(kg->build[i]);
		if (klondike_game_is_to_suite_legal_move(kg, card)) {
			current = klondike_game_legal_move_new(BuildToSuit, kg->build[i], kg->suite[card_family(card)]);
			klondike_game_legal_move_append(&moves, current);
		}
	}
	return moves;
}
KlondikeGameLegalMove *klondike_game_find_legal_moves(KlondikeGame *kg) {

	KlondikeGameLegalMove *moves = NULL;
	klondike_game_legal_move_append(&moves, klondike_game_pile_move(kg));
	klondike_game_legal_move_append(&moves, klondike_game_talon_move(kg));
	klondike_game_legal_move_append(&moves, klondike_game_build_moves(kg));
	klondike_game_legal_move_append(&moves, klondike_game_suite_moves(kg));
	return moves;
}