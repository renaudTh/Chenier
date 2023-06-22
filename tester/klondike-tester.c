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

	KlondikeGameLegalMove *move = klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_PTR_EQUAL(move->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(move->to, kg->build[1]);
	CU_ASSERT_EQUAL(move->index_from, 2);
	CU_ASSERT_EQUAL(move->type, BuildToBuild);
	klondike_game_destroy(kg);
}
void build_legal_move_with_king(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(0, 10, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 6, false));
	stack_add_card_on(kg->build[0], card_new(SPADE, 13, true));
	KlondikeGameLegalMove *move = klondike_game_build_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, BuildToBuild);
	CU_ASSERT_PTR_EQUAL(move->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(move->to, kg->build[1]);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
void talon_to_build(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 12, true));
	stack_add_card_on(kg->build[0], card_new(SPADE, 13, true));
	KlondikeGameLegalMove *move = klondike_game_talon_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, TalonToBuild);
	CU_ASSERT_PTR_EQUAL(move->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(move->to, kg->build[0]);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
void redeal_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 6, true));
	stack_add_card_on(kg->talon, card_new(SPADE, 7, true));
	KlondikeGameLegalMove *move = klondike_game_pile_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, Redeal);
	CU_ASSERT_PTR_EQUAL(move->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(move->to, kg->pile);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
void pile_to_talon_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->pile, card_new(HEART, 6, false));
	stack_add_card_on(kg->pile, card_new(SPADE, 7, false));
	KlondikeGameLegalMove *move = klondike_game_pile_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, PileToTalon);
	CU_ASSERT_PTR_EQUAL(move->from, kg->pile);
	CU_ASSERT_PTR_EQUAL(move->to, kg->talon);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
void build_to_suite_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->build[0], card_new(HEART, 10, false));
	stack_add_card_on(kg->build[0], card_new(HEART, 1, true));
	KlondikeGameLegalMove *move = klondike_game_suite_moves(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, BuildToSuit);
	CU_ASSERT_PTR_EQUAL(move->from, kg->build[0]);
	CU_ASSERT_PTR_EQUAL(move->to, kg->suite[HEART]);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
void talon_to_suite_test(void) {
	KlondikeGame *kg = klondike_game_new();
	stack_add_card_on(kg->talon, card_new(HEART, 1, true));
	KlondikeGameLegalMove *move = klondike_game_talon_move(kg);
	CU_ASSERT_PTR_NOT_NULL_FATAL(move);
	CU_ASSERT_EQUAL(move->type, TalonToSuite);
	CU_ASSERT_PTR_EQUAL(move->from, kg->talon);
	CU_ASSERT_PTR_EQUAL(move->to, kg->suite[HEART]);
	CU_ASSERT_EQUAL(move->index_from, 0);
}
static CU_TestInfo tests[] = {
    {"build legal move with three cards", build_legal_move_three_cards},
    {"build legal move with a king", build_legal_move_with_king},
    {"talon to build", talon_to_build},
    {"redeal", redeal_test},
    {"pile to talon", pile_to_talon_test},
    {"Build to suite", build_to_suite_test},
    {"Talon to suite", talon_to_suite_test},

    CU_TEST_INFO_NULL,
};

CU_SuiteInfo klondike_suite = {
    "Klondike tester", NULL, NULL, NULL, NULL, tests,
};