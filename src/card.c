
#include "card.h"

Card *card_new(Family f, uint8_t value, bool visible) {
	Card *c = malloc(sizeof(Card));
	*c = 0U;
	*c |= f;
	*c |= (value << 2);
	*c |= (visible << 7);
	return c;
}
uint8_t card_value(const Card *c) {
	return ((*c & 0x7C) >> 2);
}
Family card_family(const Card *c) {
	return ((*c & 0x3));
}
bool card_is_visible(const Card *c) {
	return ((*c & 0x80) >> 7);
}
void card_set_visible(Card *c, bool visible) {
	*c &= (visible << 7);
}
void card_flip(Card *c) {
	(*c) ^= (1 << 7);
}
void card_print(const Card *c) {
	if (card_is_visible(c)) {
		printf("%u ", card_value(c));
	} else {
		printf("# ");
	}
}
void card_destroy(Card *c) {
	if (!c) return;
	free(c);
	c = NULL;
}