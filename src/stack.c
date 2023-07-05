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
Stack *stack_create_complete_deck(int card_nb, const bool visible, const bool ace_max) {

	int min = 0, max = 0;
	if (card_nb == 52) {
		min = (ace_max) ? 1 : 0;
		max = (ace_max) ? 13 : 12;
	} else if (card_nb == 32) {
		min = 5;
		max = 13;
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
	if (from->head) {
		from->head->prev = NULL;
	} else {
		from->tail = NULL;
	}
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
	if (from->head) {
		from->head->prev = NULL;
	} else {
		from->tail = NULL;
	}
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
void stack_append_stack_on_bottom(Stack *on, Stack *to_append) {

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
void stack_append_stack_on_top(Stack *on, Stack *to_append) {

	if (!on || !to_append) return;
	if (stack_is_empty(to_append)) return;
	if (stack_is_empty(on)) {
		on->head = to_append->head;
		on->tail = to_append->tail;
		on->size = to_append->size;
	} else {
		on->head->prev = to_append->tail;
		to_append->tail->next = on->head;
		on->head = to_append->head;
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
void stack_flip(Stack *s, bool change_visibility) {
	if (!s) return;
	if (stack_is_empty(s) || stack_get_size(s) == 1) return;
	Node *runner = s->tail;
	Node *temp = NULL;
	while (runner != NULL) {
		Node *temp = runner->prev;
		runner->prev = runner->next;
		runner->next = temp;
		if (change_visibility) card_flip(runner->card);
		runner = runner->next;
	}
	temp = s->tail;
	s->tail = s->head;
	s->head = temp;
}
void stack_destroy(Stack *s) {
	if (!s) return;
	if (s->head == NULL) {
		s->size = 0;
		free(s);
		return;
	}
	if (s->head->next == NULL) {
		node_destroy(s->head);
		s->head = NULL;
		s->tail = NULL;
		s->size = 0;
		free(s);
		return;
	} else {
		Node *to_delete = s->head->next;
		s->head->next = to_delete->next;
		if (to_delete->next != NULL) {
			to_delete->next->prev = s->head;
		}
		node_destroy(to_delete);
		stack_destroy(s);
	}
}

StackIterator *stack_begin(const Stack *s) {
	if (!s) return NULL;
	return s->head;
}
StackIterator *stack_end(const Stack *s) {
	if (!s) return NULL;
	return s->tail->next;
}
StackIterator *stack_rbegin(const Stack *s) {
	if (!s) return NULL;
	return s->head;
}
StackIterator *stack_rend(const Stack *s) {
	if (!s) return NULL;
	return s->tail->prev;
}
StackIterator *stack_next(const StackIterator *it) {
	if (!it) return NULL;
	return it->next;
}
StackIterator *stack_prev(const StackIterator *it) {
	if (!it) return NULL;
	return it->prev;
}

void stack_print_raw(const Stack *s, FILE *file) {
	StackIterator *it = stack_begin(s);
	while (it != NULL) {
		fprintf(file, "%u, ", *(unsigned char *)it->card);
		it = stack_next(it);
	}
	fprintf(file, "\n");
}

Stack *stack_new_from_raw(unsigned char *table, size_t size) {
	Stack *s = stack_new_empty();
	for (size_t i = 0; i < size; i++) {
		Card *c = malloc(sizeof(Card));
		*c = table[i];
		stack_add_card_under(s, c);
	}
	return s;
}
Stack *stack_split(Stack *s, int index) {

	if (!s) return NULL;
	if (index < 0 || index >= s->size) return NULL;

	Node *runner = s->head;
	int i = 0;
	while (runner != s->tail && i < index) {
		runner = runner->next;
		i++;
	}

	Stack *ret = stack_new_empty();
	ret->head = s->head;
	ret->tail = runner;
	ret->size = index + 1;

	if (runner->next != NULL) {
		s->head = runner->next;
		s->head->prev = NULL;
		ret->tail->next = NULL;
	} else {
		s->head = NULL;
		s->tail = NULL;
	}
	s->size = s->size - (index + 1);
	return ret;
}

void stack_empty(Stack *s) {

	if (!s) return;
	if (s->head == NULL) {
		s->size = 0;
		s->tail = NULL;
		return;
	}
	if (s->head->next == NULL) {
		node_destroy(s->head);
		s->head = NULL;
		s->tail = NULL;
		s->size = 0;
		return;
	} else {
		Node *to_delete = s->head->next;
		s->head->next = to_delete->next;
		if (to_delete->next != NULL) {
			to_delete->next->prev = s->head;
		}
		node_destroy(to_delete);
		stack_empty(s);
	}
}