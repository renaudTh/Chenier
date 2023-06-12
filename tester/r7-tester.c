#include "r7/r7.h"
#include "tester.h"

static void add_next_upper_card_test(void) {

	R7Game *rg = r7_game_new();
	r7_init_without_deck(rg);

	stack_add_card_on(rg->deck, card_new(SPADE, 8, true));
	bool round = r7_game_play_card(rg);
	CU_ASSERT_TRUE(round);
	CU_ASSERT_EQUAL(stack_get_size(rg->build[SPADE]), 2);
	const Card *top = stack_read_first(rg->build[SPADE]);
	CU_ASSERT_EQUAL(card_value(top), 8);
	r7_game_destroy(rg);
}
static void add_next_lower_card_test(void) {

	R7Game *rg = r7_game_new();
	r7_init_without_deck(rg);

	stack_add_card_on(rg->deck, card_new(SPADE, 6, true));
	bool round = r7_game_play_card(rg);
	CU_ASSERT_TRUE(round);
	CU_ASSERT_EQUAL(stack_get_size(rg->build[SPADE]), 2);
	const Card *bottom = stack_read_bottom(rg->build[SPADE]);
	CU_ASSERT_EQUAL(card_value(bottom), 6);
	r7_game_destroy(rg);
}
static void add_unplayable_card_test(void) {

	R7Game *rg = r7_game_new();
	r7_init_without_deck(rg);

	stack_add_card_on(rg->deck, card_new(SPADE, 13, true));
	bool round = r7_game_play_card(rg);
	CU_ASSERT_TRUE(round);
	const Card *bottom = stack_read_bottom(rg->bin);
	CU_ASSERT_EQUAL(stack_get_size(rg->bin), 1);
	CU_ASSERT_EQUAL(card_value(bottom), 13);
	r7_game_destroy(rg);
}
static void deck_empty_test(void) {
	R7Game *rg = r7_game_new();
	r7_init_without_deck(rg);
	bool round = r7_game_play_card(rg);
	CU_ASSERT_FALSE(round);
	r7_game_destroy(rg);
}

static void winning_game_in_one_attempt(void) {
	R7Game *rg = r7_game_new();
	r7_init_winning_game_in_one_attempt(rg);
	bool win = r7_game_main_loop(rg);
	CU_ASSERT_TRUE(win);
	CU_ASSERT_EQUAL(rg->attempt_nb, 1);
	r7_game_destroy(rg);
}
static void winning_game_in_two_attempts(void) {
	R7Game *rg = r7_game_new();
	r7_init_winning_game_in_two_attempts(rg);
	bool win = r7_game_main_loop(rg);
	CU_ASSERT_TRUE(win);
	CU_ASSERT_EQUAL(rg->attempt_nb, 2);
	r7_game_destroy(rg);
}
static void losing_game(void) {
	R7Game *rg = r7_game_new();
	r7_init_losing_game(rg);
	bool win = r7_game_main_loop(rg);
	CU_ASSERT_FALSE(win);
	CU_ASSERT_EQUAL(rg->attempt_nb, 3);
	r7_game_destroy(rg);
}
static CU_TestInfo tests[] = {
    {"Add directly next upper card", add_next_upper_card_test},
    {"Add directly next lower card", add_next_lower_card_test},
    {"Add unplayable card", add_unplayable_card_test},
    {"When the deck is empty", deck_empty_test},
    {"Winning game in one attempt", winning_game_in_one_attempt},
    {"Winning game in two attempts", winning_game_in_two_attempts},
    {"Losing game", losing_game},

    CU_TEST_INFO_NULL,
};

CU_SuiteInfo r7_suite = {
    "R7 tester", NULL, NULL, NULL, NULL, tests,
};