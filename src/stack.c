#include "stack.h"

Stack *stack_new_empty() {
	Stack *s = malloc(sizeof(Stack));
	s->size = 0;
	return s;
}
Stack *stack_create_complete_deck(const bool visible) {
	Stack *s = stack_new_empty();
	for (int f = 0; f < 4; f++) {
		for (int v = MIN_VALUE; v <= MAX_VALUE; v++) {
			s->cards[s->size++] = card_new((Family)f, v, visible);
		}
	}
	return s;
}
void stack_add_card_under(Stack *s, Card *c) {
	s->size++;
	for (int i = s->size - 2; i >= 0; i--) {
		s->cards[i + 1] = s->cards[i];
	}
	s->cards[0] = c;
}
void stack_add_card_on(Stack *s, Card *c) {
	s->cards[s->size++] = c;
}
Card *stack_pop(Stack *s) {
	Card *r = s->cards[s->size - 1];
	s->size--;
	return r;
}
Card *stack_read_first(Stack *s) {
	return (s->size != 0) ? s->cards[s->size - 1] : NULL;
}
bool stack_is_empty(Stack *s) {
	return s->size == 0;
}
void stack_shuffle(Stack *s) {
	if (!s) return;
	if (s->size == 0) return;
	for (int i = s->size - 1; i > 0; i--) {
		int j = rand() % i;
		Card *t = s->cards[i];
		s->cards[i] = s->cards[j];
		s->cards[j] = t;
	}
}
void stack_destroy(Stack *s) {
	if (!s) return;
	if (s->size != 0) {
		for (int i = 0; i < s->size; i++) {
			card_destroy(s->cards[i]);
		}
	}
	free(s);
	s = NULL;
}
void stack_add_stack_on_bottom(Stack *s, Stack *toAdd, bool visibility) {
	if (s->size + toAdd->size > STACK_MAX_SIZE) {
		return;
	}
	int newSize = s->size + toAdd->size;
	for (int i = s->size - 1; i >= 0; i--) {
		s->cards[i + toAdd->size] = s->cards[i];
		s->cards[i + toAdd->size]->visible = visibility;
	}
	while (toAdd->size > 0) {
		toAdd->size--;
		s->cards[toAdd->size] = toAdd->cards[toAdd->size];
		s->cards[toAdd->size]->visible = visibility;
	}
	s->size = newSize;
}
void stack_print(const Stack *s) {

	for (int i = 0; i < s->size; i++) {
		Card *current = s->cards[i];
		card_print(current);
	}
}