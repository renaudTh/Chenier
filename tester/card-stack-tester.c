#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "card.h"
#include "stack.h"
#include "tester.h"

static void add_cards_on_empty_test(void) {

	Stack *s = stack_new_empty();
	stack_add_card_on(s, card_new(SPADE, 8, true));
	stack_add_card_on(s, card_new(SPADE, 9, true));
	stack_add_card_on(s, card_new(SPADE, 10, true));
	stack_add_card_on(s, card_new(SPADE, 11, true));
	stack_add_card_on(s, card_new(SPADE, 12, true));
	CU_ASSERT_EQUAL(stack_get_size(s), 5);
	CU_ASSERT_FALSE(stack_is_empty(s));
	const Card *c = stack_read_first(s);
	CU_ASSERT_PTR_NOT_NULL(c);
	CU_ASSERT_EQUAL(card_family(c), SPADE);
	CU_ASSERT_EQUAL(card_value(c), 12);
	CU_ASSERT_EQUAL(card_is_visible(c), true);
	stack_destroy(s);
}
static void add_cards_under_empty_test(void) {

	Stack *s = stack_new_empty();
	stack_add_card_under(s, card_new(SPADE, 8, true));
	stack_add_card_under(s, card_new(SPADE, 9, true));
	stack_add_card_under(s, card_new(SPADE, 10, true));
	stack_add_card_under(s, card_new(SPADE, 11, true));
	stack_add_card_under(s, card_new(SPADE, 12, true));
	CU_ASSERT_EQUAL(stack_get_size(s), 5);
	CU_ASSERT_FALSE(stack_is_empty(s));
	const Card *c = stack_read_first(s);
	CU_ASSERT_PTR_NOT_NULL(c);
	CU_ASSERT_EQUAL(card_family(c), SPADE);
	CU_ASSERT_EQUAL(card_value(c), 8);
	CU_ASSERT_EQUAL(card_is_visible(c), true);
	stack_destroy(s);
}

static void append_stack_on_tail_test(void) {

	Stack *s1 = stack_new_empty();
	stack_add_card_on(s1, card_new(SPADE, 8, true));
	stack_add_card_on(s1, card_new(SPADE, 9, true));
	stack_add_card_on(s1, card_new(SPADE, 10, true));
	Stack *s2 = stack_new_empty();
	stack_add_card_on(s2, card_new(HEART, 11, true));
	stack_add_card_on(s2, card_new(HEART, 12, true));
	stack_add_card_on(s2, card_new(HEART, 13, true));
	stack_append_stack_on_tail(s1, s2);
	CU_ASSERT_EQUAL(stack_get_size(s1), 6);
	stack_destroy(s1);
	stack_destroy(s2);
}

static void shuffle_test(void) {
	Stack *s = stack_new_empty();
	stack_add_card_under(s, card_new(SPADE, 8, true));
	stack_add_card_under(s, card_new(SPADE, 9, true));
	stack_add_card_under(s, card_new(SPADE, 10, true));
	stack_add_card_under(s, card_new(SPADE, 11, true));
	stack_add_card_under(s, card_new(SPADE, 12, true));
	stack_shuffle(s);
	CU_ASSERT_EQUAL(stack_get_size(s), 5);
	stack_destroy(s);
}
static void pop_from_top_to_top_test(void) {

	Stack *s1 = stack_new_empty();
	stack_add_card_on(s1, card_new(SPADE, 8, true));
	stack_add_card_on(s1, card_new(SPADE, 9, true));
	stack_add_card_on(s1, card_new(SPADE, 10, true));
	Stack *s2 = stack_new_empty();
	stack_add_card_on(s2, card_new(HEART, 11, true));
	stack_add_card_on(s2, card_new(HEART, 12, true));
	stack_add_card_on(s2, card_new(HEART, 13, true));

	stack_pop_from_top_to_top(s2, s1, false);
	const Card *c1 = stack_read_first(s1);
	const Card *c2 = stack_read_first(s2);
	CU_ASSERT_EQUAL(stack_get_size(s1), 4);
	CU_ASSERT_EQUAL(stack_get_size(s2), 2);
	CU_ASSERT_EQUAL(card_value(c1), 13);
	CU_ASSERT_EQUAL(card_value(c2), 12);
	stack_destroy(s1);
	stack_destroy(s2);
}
static void pop_from_top_to_top_on_empty_test(void) {

	Stack *s1 = stack_new_empty();
	Stack *s2 = stack_new_empty();
	stack_add_card_on(s2, card_new(HEART, 11, true));
	stack_add_card_on(s2, card_new(HEART, 12, true));
	stack_add_card_on(s2, card_new(HEART, 13, true));

	stack_pop_from_top_to_top(s2, s1, false);
	const Card *c1 = stack_read_first(s1);
	const Card *c2 = stack_read_first(s2);
	CU_ASSERT_EQUAL(stack_get_size(s1), 1);
	CU_ASSERT_EQUAL(stack_get_size(s2), 2);
	CU_ASSERT_EQUAL(card_value(c1), 13);
	CU_ASSERT_EQUAL(card_value(c2), 12);
	stack_destroy(s1);
	stack_destroy(s2);
}
static void pop_from_top_to_bottom_test(void) {

	Stack *s1 = stack_new_empty();
	stack_add_card_on(s1, card_new(SPADE, 8, true));
	stack_add_card_on(s1, card_new(SPADE, 9, true));
	stack_add_card_on(s1, card_new(SPADE, 10, true));
	Stack *s2 = stack_new_empty();
	stack_add_card_on(s2, card_new(HEART, 11, true));
	stack_add_card_on(s2, card_new(HEART, 12, true));
	stack_add_card_on(s2, card_new(HEART, 13, true));

	stack_pop_from_top_to_bottom(s2, s1, false);
	const Card *c1 = stack_read_bottom(s1);
	const Card *c2 = stack_read_first(s2);
	CU_ASSERT_EQUAL(stack_get_size(s1), 4);
	CU_ASSERT_EQUAL(stack_get_size(s2), 2);
	CU_ASSERT_EQUAL(card_value(c1), 13);
	CU_ASSERT_EQUAL(card_value(c2), 12);
	stack_destroy(s1);
	stack_destroy(s2);
}
static void pop_from_top_to_bottom_on_empty_test(void) {

	Stack *s1 = stack_new_empty();
	Stack *s2 = stack_new_empty();
	stack_add_card_on(s2, card_new(HEART, 11, true));
	stack_add_card_on(s2, card_new(HEART, 12, true));
	stack_add_card_on(s2, card_new(HEART, 13, true));

	stack_pop_from_top_to_bottom(s2, s1, false);
	const Card *c1 = stack_read_bottom(s1);
	const Card *c2 = stack_read_first(s2);
	CU_ASSERT_EQUAL(stack_get_size(s1), 1);
	CU_ASSERT_EQUAL(stack_get_size(s2), 2);
	CU_ASSERT_EQUAL(card_value(c1), 13);
	CU_ASSERT_EQUAL(card_value(c2), 12);
	stack_destroy(s1);
	stack_destroy(s2);
}

static CU_TestInfo tests[] = {
    {"Add Card on empty stack", add_cards_on_empty_test},
    {"Add card under empty stack", add_cards_under_empty_test},
    {"Append stack on tail", append_stack_on_tail_test},
    {"Shuffle test", shuffle_test},
    {"pop top/top", pop_from_top_to_top_test},
    {"pop top/top empty", pop_from_top_to_top_on_empty_test},
    {"pop top/bottom", pop_from_top_to_bottom_test},
    {"pop top/bottom empty", pop_from_top_to_bottom_on_empty_test},
    CU_TEST_INFO_NULL,
};

CU_SuiteInfo card_stack_suite = {
    "Card and stack", NULL, NULL, NULL, NULL, tests,
};