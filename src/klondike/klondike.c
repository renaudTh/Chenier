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

bool klondike_game_is_to_build_legal_move(Stack *from, Stack *to) {
	const Card *from_card = stack_read_first(from);
	if (stack_is_empty(to)) {
		return (card_value(from_card) == 13);
	}
	const Card *to_card = stack_read_first(to);
	return (card_value(to_card) == card_value(from_card) - 1) &&
	       (((card_family(from_card) % 2) & (card_family(to_card) % 2)) == 0);
}

bool klondike_game_is_to_suite_legal_move(KlondikeGame *kg, Stack *from) {
	const Card *from_card = stack_read_first(from);
	Family f = card_family(from_card);

	if (stack_is_empty(kg->suite[f])) {
		return (card_value(from_card) == 1);
	}
	const Card *to_card = stack_read_first(kg->suite[f]);
	return (card_value(to_card) == card_value(from_card) - 1);
}
KlondikeGameLegalMove *klondike_game_legal_move_new(KlondikeGameMoveType type, const Stack *from, const Stack *to) {
	KlondikeGameLegalMove *m = malloc(sizeof(KlondikeGameLegalMove));
	m->from = from;
	m->to = to;
	m->type = type;
	m->next = NULL;
	return m;
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
	// Talon to build
	KlondikeGameLegalMove *head = NULL;
	KlondikeGameLegalMove *move = NULL;
	for (int i = 0; i < 7; i++) {
		if (!klondike_game_is_to_build_legal_move(kg->pile, kg->build[i])) continue;
		move = klondike_game_legal_move_new(TalonToBuild, kg->pile, kg->build[i]);
		if (head == NULL) {
			head = move;
		}
		move = move->next;
	}
	// Talon to suite
	for (int i = 0; i < 4; i++) {
		if (!klondike_game_is_to_suite_legal_move(kg, kg->talon)) continue;
		move = klondike_game_legal_move_new(TalonToSuite, kg->pile, kg->suite[i]);
		if (head == NULL) {
			head = move;
		}
		move = move->next;
	}
	return head;
}

klondike_game_find_legal_moves(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	// Warning : if pile is empty !!
	KlondikeGameLegalMove *head = malloc(sizeof(KlondikeGameLegalMove));
	head->from = kg->pile;
	head->to = kg->talon;
	head->type = PileToTalon;
	head->next = NULL;
	KlondikeGameLegalMove *current = head->next;

	// From Build to Build
	for (int from = 0; from < 7; from++) {
		if (stack_is_empty(kg->build[from])) continue;
		for (int to = 0; to < 7; to++) {
			if (to == from) continue;
			if (klondike_game_is_to_build_legal_move(kg->build[from], kg->build[to])) {
				current = klondike_legal_move_new(BuildToBuild, kg->build[from], kg->build[to]);
				current = current->next;
			}
		}
	}

	return head;
}