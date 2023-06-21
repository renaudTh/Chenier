#include "game.h"
#include "klondike/klondike.h"
#include "tester.h"

static void build_legal_move_three_cards(void) {

	KlondikeGame *kg = klondike_game_new();

	stack_add_card_on(kg->build[0], card_new(0, 10, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 6, true));
	stack_add_card_on(kg->build[0], card_new(SPADE, 5, true));
	stack_add_card_on(kg->build[0], card_new(DIAMOND, 4, true));

	stack_add_card_on(kg->build[1], card_new(1, 10, false));
	stack_add_card_on(kg->build[1], card_new(SPADE, 7, true));

	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->build[1]);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 2);
	CU_ASSERT_EQUAL(kg->moves_list->type, BuildToBuild);
	klondike_game_destroy(kg);
}
void build_legal_move_with_king(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(0, 10, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 6, false));
	stack_add_card_on(kg->build[0], card_new(SPADE, 12, true));
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, BuildToBuild);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->build[1]);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void talon_to_build(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 12, true));
	stack_add_card_on(kg->build[0], card_new(SPADE, 13, true));
	klondike_game_talon_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, TalonToBuild);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->build[0]);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void redeal_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 6, true));
	stack_add_card_on(kg->talon, card_new(SPADE, 7, true));
	klondike_game_pile_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, Redeal);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->pile);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void pile_to_talon_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->pile, card_new(HEART, 6, false));
	stack_add_card_on(kg->pile, card_new(SPADE, 7, false));
	klondike_game_pile_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, PileToTalon);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->pile);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->talon);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void build_to_suite_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(HEART, 10, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 0, true));
	klondike_game_suite_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, BuildToSuit);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->suite[HEART]);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void talon_to_suite_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 0, true));
	klondike_game_talon_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	CU_ASSERT_EQUAL(kg->moves_list->type, TalonToSuite);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(kg->moves_list->to, kg->suite[HEART]);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 0);
	klondike_game_destroy(kg);
}
void build_to_build_same_family(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(HEART, 0, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 5, true));

	stack_add_card_on(kg->build[1], card_new(SPADE, 0, false));
	stack_add_card_on(kg->build[1], card_new(HEART, 6, true));
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NULL_FATAL(kg->moves_list);
	klondike_game_destroy(kg);
}
void build_to_build_same_color(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(HEART, 0, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 6, true));

	stack_add_card_on(kg->build[1], card_new(SPADE, 0, false));
	stack_add_card_on(kg->build[1], card_new(DIAMOND, 5, true));
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NULL_FATAL(kg->moves_list);
	klondike_game_destroy(kg);
}
static void build_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(CLUB, 10, true));
	stack_add_card_on(kg->build[1], card_new(SPADE, 5, true));
	stack_add_card_on(kg->build[2], card_new(CLUB, 9, true));
	stack_add_card_on(kg->build[3], card_new(DIAMOND, 2, true));
	stack_add_card_on(kg->build[4], card_new(CLUB, 8, true));
	stack_add_card_on(kg->build[5], card_new(CLUB, 3, true));
	stack_add_card_on(kg->build[6], card_new(DIAMOND, 9, true));
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	KlondikeGameLegalMove *it = kg->moves_list;
	int count = 0;
	while (it != NULL) {
		count++;
		it = it->next;
	}
	CU_ASSERT_EQUAL(count, 3);
	klondike_game_destroy(kg);
}
static void build_test_2(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(SPADE, 1, true));
	stack_add_card_on(kg->build[1], card_new(DIAMOND, 4, true));
	stack_add_card_on(kg->build[2], card_new(SPADE, 5, true));
	stack_add_card_on(kg->build[3], card_new(HEART, 3, true));
	stack_add_card_on(kg->build[4], card_new(DIAMOND, 2, true));
	stack_add_card_on(kg->build[5], card_new(SPADE, 3, true));
	stack_add_card_on(kg->build[6], card_new(HEART, 10, true));
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	KlondikeGameLegalMove *it = kg->moves_list;
	int count = 0;
	while (it != NULL) {
		count++;
		it = it->next;
	}
	CU_ASSERT_EQUAL(count, 4);
	klondike_game_destroy(kg);
}

static void move_several_cards(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(SPADE, 8, true));
	for (int i = 7; i > 0; i--) {
		stack_add_card_on(kg->build[1], card_new((i % 2 == 0) ? CLUB : DIAMOND, i, true));
	}
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	printf("index = %d\n", kg->moves_list->index_from);
	CU_ASSERT_EQUAL(kg->moves_list->type, BuildToBuild);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 6);
	klondike_game_move(kg->moves_list);

	CU_ASSERT_TRUE(stack_is_empty(kg->build[1]));
	stack_print(kg->build[0]);
	printf("\n");
	stack_print(kg->build[1]);
	klondike_game_destroy(kg);
}
static void move_several_cards_2(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(CLUB, 8, true));
	for (int i = 9; i > 0; i--) {
		stack_add_card_on(kg->build[1], card_new((i % 2 == 0) ? CLUB : DIAMOND, i, true));
	}
	klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(kg->moves_list);
	printf("index = %d\n", kg->moves_list->index_from);
	CU_ASSERT_EQUAL(kg->moves_list->type, BuildToBuild);
	CU_ASSERT_EQUAL(kg->moves_list->index_from, 6);
	printf("before : \n");
	stack_print(kg->build[0]);
	printf("\n");
	stack_print(kg->build[1]);
	printf("\n after : \n");
	klondike_game_move(kg->moves_list);

	CU_ASSERT_EQUAL(stack_get_size(kg->build[1]), 2);
	klondike_game_destroy(kg);
}

static void winning_test(void) {
	KlondikeGame *kg = klondike_game_new();
	for (int v = 13; v >= 0; v--) {
		for (int f = 0; f < 4; f++) {
			int stack_index = (f + (v % 2 == 1)) % 4;
			stack_add_card_on(kg->build[stack_index], card_new(f, v, true));
		}
	}

	klondike_game_destroy(kg);
}

static CU_TestInfo tests[] = {
    {"build legal move with three cards", build_legal_move_three_cards},
    {"build legal move with a king", build_legal_move_with_king},
    {"talon to build", talon_to_build},
    {"redeal", redeal_test},
    {"pile to talon", pile_to_talon_test},
    {"Build to suite", build_to_suite_test},
    {"Talon to suite", talon_to_suite_test},
    {"Build to build same family", build_to_build_same_family},
    {"Build to build same color", build_to_build_same_color},
    {"Build test", build_test},
    {"Build test 2", build_test_2},
    {"Move Several Cards", move_several_cards},
    {"Move Several Cards 2", move_several_cards_2},
    {"Winning test", winning_test},

    CU_TEST_INFO_NULL,
};

CU_SuiteInfo klondike_suite = {
    "Klondike tester", NULL, NULL, NULL, NULL, tests,
};