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
	kg->max_move_score = 0;
	kg->max_move_priority = -1;
	kg->moves_list = NULL;
	kg->dead_end = false;
	kg->lose = false;

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
	if (kg->moves_list) klondike_game_legal_move_destroy_all(&kg->moves_list);
	free(kg);
}

bool klondike_game_is_to_build_legal_move(const Card *from, Stack *to) {

	if (stack_is_empty(to)) {
		return (card_value(from) == 12);
	}
	const Card *to_card = stack_read_first(to);

	return (card_value(to_card) == card_value(from) + 1) && (card_family(to_card) != card_family(from)) &&
	       ((card_family(to_card) % 2) != (card_family(from) % 2));
}

bool klondike_game_is_to_suite_legal_move(KlondikeGame *kg, const Card *from) {

	Family f = card_family(from);
	if (stack_is_empty(kg->suite[f])) {
		return (card_value(from) == 0);
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
	m->priority = 0;
	m->score = 0;
	return m;
}

void klondike_game_legal_move_destroy_all(KlondikeGameLegalMove **moves) {
	KlondikeGameLegalMove *prev = *moves, *cur = (*moves)->next;
	while (cur != NULL) {
		prev->next = cur->next;
		free(cur);
		cur = prev->next;
	}
	if (prev) free(prev);
	*moves = NULL;
}
void klondike_game_legal_move_append(KlondikeGameLegalMove **head, KlondikeGameLegalMove *to_append) {

	if (to_append == NULL) return;

	if (*head == NULL) {
		*head = to_append;
		return;
	}

	KlondikeGameLegalMove *it = *head;
	while (it->next != NULL) {
		it = it->next;
	}
	it->next = to_append;
}
void klondike_game_pile_move(KlondikeGame *kg) {

	KlondikeGameLegalMove *m = NULL;
	if (stack_is_empty(kg->pile)) {
		m = klondike_game_legal_move_new(Redeal, kg->talon, kg->pile);
	} else {
		m = klondike_game_legal_move_new(PileToTalon, kg->pile, kg->talon);
	}
	klondike_game_legal_move_append(&kg->moves_list, m);
}

void klondike_game_talon_move(KlondikeGame *kg) {

	if (kg->talon == NULL) return;
	if (stack_is_empty(kg->talon)) return;
	const Card *c = stack_read_first(kg->talon);
	// Talon to build
	KlondikeGameLegalMove *move = NULL;
	for (int i = 0; i < 7; i++) {
		if (!klondike_game_is_to_build_legal_move(c, kg->build[i])) continue;
		move = klondike_game_legal_move_new(TalonToBuild, kg->talon, kg->build[i]);
		klondike_game_legal_move_append(&kg->moves_list, move);
	}
	// Talon to suite
	if (klondike_game_is_to_suite_legal_move(kg, c)) {
		move = klondike_game_legal_move_new(TalonToSuite, kg->talon, kg->suite[card_family(c)]);
		klondike_game_legal_move_append(&kg->moves_list, move);
	}
}

void klondike_game_build_to_build(KlondikeGame *kg, Stack *from) {

	if (!from || stack_is_empty(from)) return;
	StackIterator *it = stack_begin(from);
	KlondikeGameLegalMove *move = NULL;
	int index = 0;
	while (it != stack_end(from) && card_is_visible(it->card)) {
		for (int i = 0; i < 7; i++) {
			if (from == kg->build[i]) continue;
			if (it == from->tail && card_value(it->card) == 12) continue;
			if (!klondike_game_is_to_build_legal_move(it->card, kg->build[i])) continue;
			move = klondike_game_legal_move_new(BuildToBuild, from, kg->build[i]);
			move->index_from = index;
			klondike_game_legal_move_append(&kg->moves_list, move);
		}
		it = stack_next(it);
		index++;
	}
}

void klondike_game_build_moves(KlondikeGame *kg) {

	for (int i = 0; i < 7; i++) {
		klondike_game_build_to_build(kg, kg->build[i]);
	}
}

bool klondike_game_init_several(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	stack_add_card_on(kg->build[0], card_new(SPADE, 8, true));
	for (int i = 9; i > 0; i--) {
		stack_add_card_on(kg->build[1], card_new((i % 2 == 0) ? CLUB : DIAMOND, i, true));
	}
	return true;
}
void _check_flip_prev_card(Stack *from) {
	if (stack_is_empty(from)) return;
	if (!card_is_visible(from->head->card)) card_flip(from->head->card);
}

void _build_to_build_move(KlondikeGameLegalMove *move) {

	if (move->index_from == 0) stack_pop_from_top_to_top(move->from, move->to, false);
	else {
		Stack *temp = stack_split(move->from, move->index_from);
		stack_append_stack_on_top(move->to, temp);
		stack_destroy(temp);
	}
	_check_flip_prev_card(move->from);
}

void _build_to_suit_move(KlondikeGameLegalMove *move) {
	stack_pop_from_top_to_top(move->from, move->to, false);
	_check_flip_prev_card(move->from);
}
void _talon_to_build_move(KlondikeGameLegalMove *move) {
	stack_pop_from_top_to_top(move->from, move->to, false);
}
void _talon_to_suite_move(KlondikeGameLegalMove *move) {
	stack_pop_from_top_to_top(move->from, move->to, false);
}
void _pile_to_talon_move(KlondikeGameLegalMove *move) {
	stack_pop_from_top_to_top(move->from, move->to, true);
}
void _redeal_move(KlondikeGameLegalMove *move) {
	stack_flip(move->from, true);
	stack_append_stack_on_bottom(move->to, move->from);
}
void klondike_game_move(KlondikeGameLegalMove *move) {

	if (!move) return;
	switch (move->type) {
		case BuildToBuild:
			_build_to_build_move(move);
			break;
		case BuildToSuit:
			_build_to_suit_move(move);
			break;
		case TalonToBuild:
			_talon_to_build_move(move);
			break;
		case TalonToSuite:
			_talon_to_suite_move(move);
			break;
		case PileToTalon:
			_pile_to_talon_move(move);
			break;
		case Redeal:
			_redeal_move(move);
			break;
		default:
			return;
	}
}

void klondike_game_suite_moves(KlondikeGame *kg) {
	KlondikeGameLegalMove *current = NULL;
	for (int i = 0; i < 7; i++) {
		if (stack_is_empty(kg->build[i])) continue;
		const Card *card = stack_read_first(kg->build[i]);
		if (klondike_game_is_to_suite_legal_move(kg, card)) {
			current = klondike_game_legal_move_new(BuildToSuit, kg->build[i], kg->suite[card_family(card)]);
			klondike_game_legal_move_append(&kg->moves_list, current);
		}
	}
}
KlondikeGameLegalMove *klondike_game_find_legal_moves(KlondikeGame *kg) {

	klondike_game_pile_move(kg);
	klondike_game_talon_move(kg);
	klondike_game_build_moves(kg);
	klondike_game_suite_moves(kg);
	KlondikeGameLegalMove *it = kg->moves_list;
	while (it != NULL) {
		klondike_game_assign_priority(kg, it);
		it = it->next;
	}
	return kg->moves_list;
}

bool klondike_game_init_random(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;

	stack_destroy(kg->pile);
	kg->pile = stack_create_complete_deck(52, false, false);
	stack_shuffle(kg->pile);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < i; j++) {
			stack_pop_from_top_to_top(kg->pile, kg->build[i], false);
		}
		stack_pop_from_top_to_top(kg->pile, kg->build[i], true);
	}

	return true;
}

KlondikeGameLegalMove *klondike_game_find_best_move(KlondikeGame *kg, KlondikeGameLegalMove *moves) {

	KlondikeGameLegalMove *current = moves;
	while (current != NULL) {
		if (kg->max_move_score > 0 && (current->score == kg->max_move_score)) {
			break;
		} else if (kg->max_move_score == 0 && (current->priority == kg->max_move_priority)) {
			break;
		}
		current = current->next;
	}
	return current;
}
// static char *legal_move_to_string(KlondikeGameMoveType type) {
// 	switch (type) {
// 		case BuildToBuild:
// 			return "BuildToBuild";
// 		case BuildToSuit:
// 			return "BuildToSuite";
// 		case TalonToBuild:
// 			return "TalonToBuild";
// 		case TalonToSuite:
// 			return "TalonToSuite";
// 		case PileToTalon:
// 			return "PileToTalon";
// 		case SuiteToBuild:
// 			return "SuiteToBuild";
// 		case Redeal:
// 			return "Redeal";
// 	}
// 	return "Unknown type of move";
// }
// static void print_moves(KlondikeGameLegalMove *m, bool one) {

// 	if (m == NULL) return;

// 	KlondikeGameLegalMove *it = m;
// 	if (one) {
// 		printf("Type : %s\n", legal_move_to_string(it->type));
// 		printf("Score : %u\n", it->score);
// 		printf("Priority : %u\n", it->priority);
// 		return;
// 	}
// 	printf("============================== Move =================================\n");
// 	while (it != NULL) {

// 		printf("Type : %s\n", legal_move_to_string(it->type));
// 		printf("Score : %u\n", it->score);
// 		printf("Priority : %u\n", it->priority);
// 		printf("Index : %u\n\n", it->index_from);

// 		it = it->next;
// 	}
// }

bool klondike_game_is_matching_queen_available(KlondikeGame *kg, Family f) {

	StackIterator *it = NULL;
	// Search in Pile;
	it = stack_begin(kg->pile);
	while (it != stack_end(kg->pile)) {
		if (card_family(it->card) == f && card_value(it->card) == 12) return true;
		it = stack_next(it);
	}
	// Search in Talon
	it = stack_begin(kg->talon);
	while (it != stack_end(kg->talon)) {
		if (card_family(it->card) == f && card_value(it->card) == 12) return true;
		it = stack_next(it);
	}
	// Search in Build;
	for (int i = 0; i < 7; i++) {
		it = stack_begin(kg->build[i]);
		while (it != stack_end(kg->build[i]) && card_is_visible(it->card)) {
			if (card_family(it->card) == f && card_value(it->card) == 12) return true;
			it = stack_next(it);
		}
	}
	// Search in Suite
	it = stack_begin(kg->suite[f]);
	while (it != stack_end(kg->suite[f])) {
		if (card_value(it->card) == 12) return true;
		it = stack_next(it);
	}
	return false;
}

void klondike_game_update_max_priority(KlondikeGame *kg, int priority) {
	if (priority > kg->max_move_priority) kg->max_move_priority = priority;
}

void klondike_game_assign_priority(KlondikeGame *kg, KlondikeGameLegalMove *move) {

	if (move->type == TalonToBuild || move->type == TalonToSuite || move->type == BuildToSuit) {
		move->score = 5;
	} else {
		move->score = 0;
	}
	if (move->score > kg->max_move_score) kg->max_move_score = move->score;
	if (move->type == BuildToBuild) {
		if (move->index_from + 1 > stack_get_size(move->from)) {
			move->priority = 0;
			klondike_game_update_max_priority(kg, move->priority);
			return;
		}
		if (move->index_from + 1 == stack_get_size(move->from)) {
			move->priority = 1;
			klondike_game_update_max_priority(kg, move->priority);
			return;
		}
		int i = 0;
		StackIterator *it = stack_begin(move->from);
		while (it != stack_end(move->from) && i < move->index_from) {
			it = stack_next(it);
			i++;
		}
		if (!card_is_visible(it->next->card)) {
			move->priority = stack_get_size(move->from) - move->index_from + 1;
		} else {
			move->priority = 0;
		}
		klondike_game_update_max_priority(kg, move->priority);
		return;
	}
	if (move->type == BuildToBuild) {
		const Card *c = stack_read_first(move->from);
		if (card_value(c) == 13) {
			bool queen_available = klondike_game_is_matching_queen_available(kg, card_family(c));
			if (queen_available) {
				move->priority = 1;
			} else {
				move->priority = -1;
			}
		} else {
			move->priority = 1;
		}
	}
	move->priority = 0;
	klondike_game_update_max_priority(kg, move->priority);
}
void klondike_game_check_dead_end(KlondikeGame *kg, KlondikeGameLegalMove *best) {
	if (best->type == Redeal && kg->dead_end) {
		kg->lose = true;
	} else if (best->type == Redeal && !kg->dead_end) {
		kg->dead_end = true;
	} else if (best->type != Redeal && best->type != PileToTalon) {
		kg->dead_end = false;
	}
}
GameActionResult klondike_game_play_card(void *game) {
	GameActionResult res = {0};
	KlondikeGame *kg = (KlondikeGame *)game;
	KlondikeGameLegalMove *moves = klondike_game_find_legal_moves(kg);
	KlondikeGameLegalMove *best = klondike_game_find_best_move(kg, moves);
	klondike_game_check_dead_end(kg, best);
	klondike_game_move(best);
	res.stateChanged = true;
	res.iterate = false;
	return res;
}
GameActionResult klondike_game_iterate(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	kg->max_move_priority = -1;
	kg->max_move_score = 0;
	klondike_game_legal_move_destroy_all(&kg->moves_list);
	GameActionResult res = {0};
	res.iterate = false;
	res.stateChanged = false;
	return res;
}
bool klondike_game_ended(void *game) {

	KlondikeGame *kg = (KlondikeGame *)game;
	return klondike_game_won(game) || kg->lose;
}
bool klondike_game_won(void *game) {

	KlondikeGame *kg = (KlondikeGame *)game;
	for (int i = 0; i < 4; i++) {
		if (stack_get_size(kg->suite[i]) < 13) return false;
	}
	return true;
}
bool klondike_game_init_winning(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	for (int v = 12; v >= 0; v--) {
		for (int f = 0; f < 4; f++) {
			int stack_index = (f + (v % 2 == 1)) % 4;
			stack_add_card_on(kg->build[stack_index], card_new(f, v, true));
		}
	}
	return true;
}
void klondike_game_save_state(KlondikeGame *kg, char *file_name) {

	FILE *file = fopen(file_name, "w");
	stack_print_raw(kg->pile, file);
	stack_print_raw(kg->talon, file);
	for (int i = 0; i < 7; i++) {
		stack_print_raw(kg->build[i], file);
	}
	for (int i = 0; i < 4; i++) {
		stack_print_raw(kg->suite[i], file);
	}
	fclose(file);
}

bool klondike_game_init_non_trivial_winning(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	unsigned char pile[] = {44, 46, 2,  24, 33, 14, 12, 18, 10, 15, 19, 27,
	                        48, 38, 39, 31, 26, 5,  16, 17, 49, 51, 3,  20};
	stack_destroy(kg->pile);
	kg->pile = stack_new_from_raw(pile, 24);
	unsigned char build0[] = {139};
	unsigned char build1[] = {156, 40};
	unsigned char build2[] = {163, 23, 8};
	unsigned char build3[] = {175, 21, 25, 29};
	unsigned char build4[] = {132, 9, 13, 50, 43};
	unsigned char build5[] = {158, 41, 0, 6, 32, 36};
	unsigned char build6[] = {170, 37, 7, 22, 1, 34, 45};

	stack_destroy(kg->build[0]);
	kg->build[0] = stack_new_from_raw(build0, 1);
	stack_destroy(kg->build[1]);
	kg->build[1] = stack_new_from_raw(build1, 2);
	stack_destroy(kg->build[2]);
	kg->build[2] = stack_new_from_raw(build2, 3);
	stack_destroy(kg->build[3]);
	kg->build[3] = stack_new_from_raw(build3, 4);
	stack_destroy(kg->build[4]);
	kg->build[4] = stack_new_from_raw(build4, 5);
	stack_destroy(kg->build[5]);
	kg->build[5] = stack_new_from_raw(build5, 6);
	stack_destroy(kg->build[6]);
	kg->build[6] = stack_new_from_raw(build6, 7);

	return true;
}
bool klondike_game_reinit(void *game) {
	KlondikeGame *kg = (KlondikeGame *)game;
	if (kg->pile) stack_empty(kg->pile);
	if (kg->talon) stack_empty(kg->talon);
	for (int i = 0; i < 4; i++) {
		if (kg->suite[i]) stack_empty(kg->suite[i]);
	}
	for (int i = 0; i < 7; i++) {
		if (kg->build[i]) stack_empty(kg->build[i]);
	}
	if (kg->moves_list) klondike_game_legal_move_destroy_all(&kg->moves_list);
	kg->lose = false;
	kg->dead_end = false;
	kg->max_move_priority = -1;
	kg->max_move_score = 0;
	return klondike_game_init_random(game);
}

CardGame klondike_winning = {
    .ended = klondike_game_ended,
    .init = klondike_game_init_non_trivial_winning,
    .iterate = klondike_game_iterate,
    .name = "The winning Klondike",
    .play_card = klondike_game_play_card,
    .won = klondike_game_won,
    .type = CardGameTypeKlondike,
    .reinit = klondike_game_reinit,
};
CardGame klondike_random = {
    .ended = klondike_game_ended,
    .init = klondike_game_init_random,
    .iterate = klondike_game_iterate,
    .name = "The random Klondike",
    .play_card = klondike_game_play_card,
    .won = klondike_game_won,
    .type = CardGameTypeKlondike,
    .reinit = klondike_game_reinit,
};