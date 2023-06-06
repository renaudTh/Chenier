
#include "card.h"

Card *card_new(Family f, int value, bool visible) {
	Card *c = malloc(sizeof(Card));
	c->f = f;
	c->value = value;
	c->visible = visible;
	return c;
}
void card_flip(Card *c) {
	c->visible = !c->visible;
}
Card *card_copy(const Card *c) {
	return card_new(c->f, c->value, c->visible);
}
void card_print(const Card *c) {

	if (c->visible) printf("%02d ", c->value);
	else printf("# ");
}
void card_destroy(Card *c) {
	if (c) {
		free(c);
		c = NULL;
	}
}