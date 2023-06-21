#ifndef CARD_H
#define CARD_H

#define STACK_MAX_SIZE 32
#define MIN_VALUE 7
#define MAX_VALUE 14

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum Family_t {

	/**
	 * ♣️
	 */
	CLUB = 0,
	/**
	 *♦️
	 */
	DIAMOND = 1,
	/**
	 * ♠️
	 */
	SPADE = 2,
	/** ♥️*/
	HEART = 3,

} Family;
typedef int PlayerId;
typedef unsigned char uint8_t;
typedef uint8_t Card;

Card *card_new(Family family, uint8_t value, bool visible);
uint8_t card_value(const Card *c);
Family card_family(const Card *c);
bool card_is_visible(const Card *c);
void card_flip(Card *c);
void card_set_visible(Card *c, bool visible);
void card_print(const Card *c);
void card_destroy(Card *c);
#endif // CARD_H