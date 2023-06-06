#ifndef CARD_H
#define CARD_H

#define STACK_MAX_SIZE 32
#define MIN_VALUE 8
#define MAX_VALUE 15

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int PlayerId;

typedef enum Family_t {
	Cubes = 0,
	Diamonds = 1,
	Hearts = 2,
	Spades = 3,
} Family;

typedef struct Card_t {
	Family f;
	int value;
	bool visible;
} Card;

Card *card_new(Family f, int value, bool visible);
void card_flip(Card *c);
Card *card_copy(const Card *c);
void card_print(const Card *c);
void card_destroy(Card *c);
#endif // CARD_H