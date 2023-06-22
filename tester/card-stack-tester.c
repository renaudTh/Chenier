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
	stack_append_stack_on_bottom(s1, s2);
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

static void stack_iterator_test(void) {

	Stack *s = stack_new_empty();
	stack_add_card_on(s, card_new(SPADE, 8, true));
	stack_add_card_on(s, card_new(SPADE, 9, true));
	stack_add_card_on(s, card_new(SPADE, 10, true));
	stack_add_card_on(s, card_new(SPADE, 11, true));
	stack_add_card_on(s, card_new(SPADE, 12, true));

	StackIterator *it = stack_begin(s);
	int i = 12;
	while (it != stack_end(s)) {
		CU_ASSERT_PTR_NOT_NULL(it);
		CU_ASSERT_EQUAL(card_value(it->card), i);
		it = stack_next(it);
		i--;
	}

	it = stack_rend(s);
	i = 8;
	while (it != stack_rbegin(s)) {
		CU_ASSERT_PTR_NOT_NULL(it);
		CU_ASSERT_EQUAL(card_value(it->card), i);
		it = stack_prev(it);
		i++;
	}
}

static void stack_split_test(void) {
	Stack *s1 = stack_new_empty();
	stack_add_card_on(s1, card_new(SPADE, 8, true));
	stack_add_card_on(s1, card_new(SPADE, 9, true));
	stack_add_card_on(s1, card_new(SPADE, 10, true));
	stack_add_card_on(s1, card_new(HEART, 11, true));
	stack_add_card_on(s1, card_new(HEART, 12, true));
	stack_add_card_on(s1, card_new(HEART, 13, true));

	Stack *s2 = stack_split(s1, 2);

	CU_ASSERT_EQUAL(stack_get_size(s1), 2);
	int i = 13;
	for (StackIterator *it = stack_begin(s1); it != stack_end(s1); it = stack_next(it)) {
		CU_ASSERT_EQUAL(card_value(it->card), i);
		i--;
	}
	i = 11;
	for (StackIterator *it = stack_begin(s2); it != stack_end(s2); it = stack_next(it)) {
		CU_ASSERT_EQUAL(card_value(it->card), i);
		i--;
	}
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
    {"stack const iterator", stack_iterator_test},
    {"stack append iterator from", stack_split_test},

    CU_TEST_INFO_NULL,
};

CU_SuiteInfo card_stack_suite = {
    "Card and stack", NULL, NULL, NULL, NULL, tests,
};