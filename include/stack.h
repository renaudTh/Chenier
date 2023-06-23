#ifndef STACK_H
#define STACK_H

#include "card.h"

typedef struct Node_t {
	Card *card;
	struct Node_t *next;
	struct Node_t *prev;
} Node;

/** The structure that represents a stack. This is a doubly linked-list with a head pointer and a tail pointer, keeping
 * track of the number of elements in the stack.
 **/
typedef struct Stack_t {
	/** Head pointer*/
	Node *head;
	/** Tail pointer*/
	Node *tail;
	/** Size of the stack*/
	uint8_t size;
} Stack;

typedef Node StackIterator;

/**
 * Creates an empty stack.
 * @return A #Stack.
 */
Stack *stack_new_empty();
/**
 * Creates a complete deck ordered by families and values.
 * @param card_nb The number of card deck. has to be 32 or 52.
 * @param visible if true, each card is set to visible.
 * @return a stack with 32 or 52 cards, @maybenil.
 */
Stack *stack_create_complete_deck(int card_nb, const bool visible);
/**
 * Add a card at the bottom of a stack.
 * @param s The stack to append the card
 * @param card The card to append
 */
void stack_add_card_under(Stack *s, Card *c);
/**
 * Add a card at the top of a stack.
 * @param s The stack to append the card
 * @param card The card to append
 */
void stack_add_card_on(Stack *s, Card *c);
/**
 * Return the size of the stack.
 * @param s current stack
 * @return The size of the stack. 0 if empty.
 */
uint8_t stack_get_size(const Stack *s);
/**
 * Return a read-only card that is at the top of the stack.
 * @param s The stack
 * @return a constant Card object or NULL if the stack is empty.
 */
const Card *stack_read_first(const Stack *s);
/**
 * Return a read-only card that is at the bottom of the stack.
 * @param s The stack
 * @return a constant Card object or NULL if the stack is empty.
 */
const Card *stack_read_bottom(const Stack *s);
/**
 * Check if a stack is empty.
 * @param s The stack
 * @return true if and only if s is empty
 */
bool stack_is_empty(const Stack *s);
/**
 * Schuffles a stack using a Fisher-Yates shuffling algorithm.
 * @param s The stack to shuffle.
 */
void stack_shuffle(Stack *s);
/**
 * Free the memory used by the stack. Free all cards of the stack if needed.
 * @param s The stack
 */
void stack_destroy(Stack *s);
/**
 * Pop the first card of a stack and add it to the top of another.
 * @param from The stack to take the card from.
 * @param to The destination stack.
 * @param flip If true, change the visibility of the card.
 */
void stack_pop_from_top_to_top(Stack *from, Stack *to, const bool flip);
/**
 * Pop the last card of a stack and add it to the bottom of another.
 * @param from The stack to take the card from.
 * @param to The destination stack.
 * @param flip If true, change the visibility of the card.
 */
void stack_pop_from_top_to_bottom(Stack *from, Stack *to, const bool flip);
/**
 * Append a stack to the bottom of another.
 * @param s The destination stack
 * @param toAdd The stack to append.
 */
void stack_append_stack_on_bottom(Stack *s, Stack *toAdd);
/**
 * Append a stack to the top of another.
 * @param s The destination stack
 * @param toAdd The stack to append.
 */
void stack_append_stack_on_top(Stack *s, Stack *toAdd);
/**
 * Set the visibility for each card of the stack
 * @param s The stack
 * @param visibility If true, cards are visible.
 */
void stack_set_visibility(Stack *s, const bool visibility);
/**
 * Flips a stack. The head become the tail and the tail becomes the head. All links are exchanged and change visibility
 * if needed.
 * @param s The stack to flip
 * @param change_visibility If true, all visibilities are changed.
 */
void stack_flip(Stack *s, bool change_visibility);
/**
 * Write to stdout the state of a Stack.
 * @param s The stack.
 */
void stack_print(const Stack *s);

StackIterator *stack_begin(const Stack *s);
StackIterator *stack_rbegin(const Stack *s);
StackIterator *stack_end(const Stack *s);
StackIterator *stack_rend(const Stack *s);
StackIterator *stack_next(const StackIterator *it);
StackIterator *stack_prev(const StackIterator *it);
Stack *stack_split(Stack *s, int index);
#endif // STACK_H