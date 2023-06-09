#include "stack.h"

static Node *node_new(Card *c) {
	Node *n = malloc(sizeof(Node));
	n->card = c;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

static void node_destroy(Node *n) {
	if (!n) return;
	if (n->card) card_destroy(n->card);
	n->next = NULL;
	n->prev = NULL;
	free(n);
	n = NULL;
}
Stack *stack_new_empty() {
	Stack *s = malloc(sizeof(Stack));
	s->head = NULL;
	s->tail = NULL;
	s->size = 0;
	return s;
}
uint8_t stack_get_size(const Stack *s) {
	return s->size;
}
void stack_add_card_under(Stack *s, Card *c) {

	if (!s || !c) return;
	Node *n = node_new(c);
	if (s->size == 0) {
		s->head = n;
		s->tail = n;
	} else {
		s->tail->next = n;
		n->prev = s->tail;
		s->tail = n;
	}
	s->size++;
}
Stack *stack_create_complete_deck(int card_nb, const bool visible) {

	int min = 0, max = 0;
	if (card_nb == 52) {
		min = 2;
		max = 14;
	} else if (card_nb == 32) {
		min = 7;
		max = 14;
	} else {
		return NULL;
	}
	Stack *s = stack_new_empty();
	for (int f = 0; f < 4; f++) {
		for (int v = min; v <= max; v++) {
			Card *c = card_new(f, v, visible);
			stack_add_card_on(s, c);
		}
	}
	return s;
}
void stack_add_card_on(Stack *s, Card *c) {
	if (!s || !c) return;
	Node *n = node_new(c);
	if (stack_is_empty(s)) {
		s->head = n;
		s->tail = n;
	} else {
		s->head->prev = n;
		n->next = s->head;
		s->head = n;
	}
	s->size++;
}
void stack_pop_from_top_to_top(Stack *from, Stack *to, const bool flip) {
	if (!from || !to) return;
	if (stack_is_empty(from)) return;

	Node *to_add = from->head;
	if (flip) {
		card_flip(to_add->card);
	}
	from->head = from->head->next;
	from->head->prev = NULL;
	from->size--;
	if (stack_is_empty(to)) {
		to->head = to_add;
		to->tail = to_add;
		to_add->prev = NULL;
		to_add->next = NULL;
	} else {
		to_add->next = to->head;
		to_add->prev = NULL;
		to->head->prev = to_add;
		to->head = to_add;
	}
	to->size++;
}
void stack_pop_from_top_to_bottom(Stack *from, Stack *to, const bool flip) {
	if (!from || !to) return;
	if (stack_is_empty(from)) return;
	Node *to_add = from->head;
	if (flip) {
		card_flip(to_add->card);
	}
	from->head = from->head->next;
	from->head->prev = NULL;
	from->size--;
	if (stack_is_empty(to)) {
		to->head = to_add;
		to->tail = to_add;
		to_add->prev = NULL;
		to_add->next = NULL;
	} else {
		to_add->prev = to->tail;
		to_add->next = NULL;
		to->tail->next = to_add;
		to->tail = to_add;
	}
	to->size++;
}
bool stack_is_empty(const Stack *s) {
	return s->size == 0;
}
const Card *stack_read_first(const Stack *s) {
	if (!s) return NULL;
	if (stack_is_empty(s)) return NULL;
	return s->head->card;
}
const Card *stack_read_bottom(const Stack *s) {
	if (!s) return NULL;
	if (stack_is_empty(s)) return NULL;
	return s->tail->card;
}
void stack_shuffle(Stack *s) {
	if (!s) return;
	if (stack_is_empty(s)) return;
	Card **shuffle_table = NULL;
	shuffle_table = malloc(sizeof(Card *) * s->size);
	Node *runner = s->head;
	int i = 0;
	while (runner != NULL) {
		shuffle_table[i++] = runner->card;
		runner = runner->next;
	}
	for (int j = s->size - 1; j > 0; j--) {
		int k = rand() % j;
		Card *t = shuffle_table[j];
		shuffle_table[j] = shuffle_table[k];
		shuffle_table[k] = t;
	}
	runner = s->head;
	i = 0;
	while (runner != NULL) {
		runner->card = shuffle_table[i++];
		runner = runner->next;
	}
	free(shuffle_table);
}
void stack_print(const Stack *s) {
	if (!s) return;
	if (stack_is_empty(s)) return;
	Node *runner = s->head;
	while (runner != NULL) {
		card_print(runner->card);
		runner = runner->next;
	}
}
void stack_append_stack_on_tail(Stack *on, Stack *to_append) {

	if (!on || !to_append) return;
	if (stack_is_empty(to_append)) return;
	if (stack_is_empty(on)) {
		on->head = to_append->head;
		on->tail = to_append->tail;
		on->size = to_append->size;
	} else {
		on->tail->next = to_append->head;
		to_append->head->prev = on->tail;
		on->tail = to_append->tail;
		on->size += to_append->size;
	}
	to_append->head = NULL;
	to_append->tail = NULL;
	to_append->size = 0;
}
void stack_set_visibility(Stack *s, const bool visibility) {
	if (!s) return;
	if (stack_is_empty(s)) return;
	Node *runner = s->head;
	while (runner->next != NULL) {
		card_set_visible(runner->card, visibility);
		runner = runner->next;
	}
}
void stack_destroy(Stack *s) {
	if (!s) return;
	Node *runner = s->head;
	while (runner != NULL) {
		s->head = runner->next;
		node_destroy(runner);
		s->size--;
		runner = s->head;
	}
	s->tail = NULL;
	free(s);
	s = NULL;
}
