#ifndef STACK_H
#define STACK_H

#include "card.h"

typedef struct Stack_t {
	int size;
	Card *cards[STACK_MAX_SIZE];
} Stack;

Stack *stack_new_empty();
Stack *stack_create_complete_deck(const bool visible);
void stack_add_card_under(Stack *s, Card *c);
void stack_add_card_on(Stack *s, Card *c);
Card *stack_pop(Stack *s);
Card *stack_read_first(Stack *s);
bool stack_is_empty(Stack *s);
void stack_shuffle(Stack *s);
void stack_destroy(Stack *s);
void stack_add_stack_on_bottom(Stack *s, Stack *toAdd, bool visibility);
void stack_print(const Stack *s);

#endif // STACK_H