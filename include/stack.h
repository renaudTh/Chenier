#ifndef STACK_H
#define STACK_H

#include "card.h"

typedef struct Node_t {
	Card *card;
	struct Node_t *next;
	struct Node_t *prev;
} Node;

typedef struct Stack_t {
	Node *head;
	Node *tail;
	uint8_t size;
} Stack;

typedef Node StackIterator;

Stack *stack_new_empty();
Stack *stack_create_complete_deck(int card_nb, const bool visible);
void stack_add_card_under(Stack *s, Card *c);
void stack_add_card_on(Stack *s, Card *c);
uint8_t stack_get_size(const Stack *s);
const Card *stack_read_first(const Stack *s);
const Card *stack_read_bottom(const Stack *s);

bool stack_is_empty(const Stack *s);
void stack_shuffle(Stack *s);
void stack_destroy(Stack *s);
void stack_pop_from_top_to_top(Stack *from, Stack *to, const bool flip);
void stack_pop_from_top_to_bottom(Stack *from, Stack *to, const bool flip);

void stack_append_stack_on_bottom(Stack *s, Stack *toAdd);
void stack_append_stack_on_top(Stack *s, Stack *toAdd);

void stack_set_visibility(Stack *s, const bool visibility);
void stack_flip(Stack *s);
void stack_print(const Stack *s);

StackIterator *stack_begin(const Stack *s);
StackIterator *stack_end(const Stack *s);
StackIterator *stack_next(const StackIterator *it);
StackIterator *stack_prev(const StackIterator *it);

#endif // STACK_H