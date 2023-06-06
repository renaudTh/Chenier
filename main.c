#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define STACK_MAX_SIZE 32
#define MIN_VALUE 8
#define MAX_VALUE 15

typedef int PlayerId;

typedef enum Family_t{
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
Card * card_new(Family f, int value, bool visible){
    Card *c = malloc(sizeof(Card));
    c->f = f;
    c->value = value;
    c->visible = visible;
    return c;
}
void card_flip(Card *c){
    c->visible = !c->visible;
}
Card *card_copy(const Card *c) { return card_new(c->f, c->value, c->visible); }
void card_print(const Card *c) { printf("%02d ", c->value); }
void card_destroy(Card * c){
    if(c){
        free(c);
        c = NULL;
    }
}
typedef struct Stack_t {
    int size;
    Card * cards[STACK_MAX_SIZE];
} Stack;

Stack * stack_new_empty(){
    Stack * s = malloc(sizeof(Stack));
    s->size = 0;
    return s;
}
Stack * stack_create_complete_deck(const bool visible){
    Stack * s = stack_new_empty();
    for(int f = 0; f < 4; f++){
        for(int v = MIN_VALUE; v <= MAX_VALUE; v++){
            s->cards[s->size++] = card_new((Family)f, v, visible);
        }
    }
    return s;
}
void stack_add_card_under(Stack *s, Card *c){
    s->size++;
    for(int i = s->size - 2; i >=0; i--){
        s->cards[i+1] = s->cards[i];
    }
    s->cards[0] = c;
}
void stack_add_card_on(Stack *s, Card *c){
    s->cards[s->size++] = c;
}
Card * stack_pop(Stack *s){
    Card *r = s->cards[s->size - 1];
    s->size--;
    return r;
}
Card * stack_read_first(Stack *s){
    return (s->size != 0) ? s->cards[s->size - 1] : NULL;
}
bool stack_is_empty(Stack *s){
    return s->size == 0;
}
void stack_shuffle(Stack *s){
    if(!s) return;
    if(s->size == 0) return;
    for(int i = s->size - 1; i > 0; i--){
        int j = rand() % i;
        Card * t = s->cards[i];
        s->cards[i] = s->cards[j];
        s->cards[j] = t;
    }
}
void stack_destroy(Stack *s){
    if(!s) return;
    if(s->size != 0){
        for(int i = 0; i < s->size; i++){
            card_destroy(s->cards[i]);
        }
    }
    free(s);
    s = NULL;
}
void stack_add_stack_on_bottom(Stack *s, Stack *toAdd, bool visibility){
    if(s->size + toAdd->size > STACK_MAX_SIZE){
        return;
    }
    int newSize = s->size + toAdd->size;
    for(int i = s->size - 1; i >= 0; i--){
        s->cards[i+toAdd->size] = s->cards[i];
        s->cards[i+toAdd->size]->visible = visibility;
    }
    while(toAdd->size > 0){
        toAdd->size--;
        s->cards[toAdd->size] = toAdd->cards[toAdd->size];
        s->cards[toAdd->size]->visible = visibility;
    }
    s->size = newSize;
}
void stack_print(const Stack * s){

    for(int i = 0; i < s->size; i++){
        Card * current = s->cards[i];
        card_print(current);
    }
}

typedef struct BattleGame_t {
    int nb_players;
    Stack ** players;
    Stack ** game;
    PlayerId *playing_id;
    int nb_playing;
} BattleGame;

BattleGame * battle_game_new(int nb_players){
    BattleGame * b = malloc(sizeof(BattleGame));
    b->nb_players = nb_players;
    b->players = malloc(nb_players * sizeof(Stack));
    b->game = malloc(sizeof(Stack) * nb_players);
    b->playing_id = malloc(sizeof(PlayerId) * nb_players);
    b->nb_playing = nb_players;
    for (int i = 0; i < nb_players; i++) {
        b->players[i] = stack_new_empty();
        b->game[i] = stack_new_empty();
        b->playing_id[i] = i + 1;
    }
    return b;
}
void battle_game_init(BattleGame *bg){
    if(!bg) return;
    Stack *deck = stack_create_complete_deck(false);
    stack_shuffle(deck);
    int j = 0;
    while (!stack_is_empty(deck)) {
        Card *current = stack_pop(deck);
        stack_add_card_on(bg->players[j % bg->nb_players], current);
        j++;
    }
    stack_destroy(deck);
}

void battle_game_destroy(BattleGame *bg){
    for (int i = 0; i < bg->nb_players; i++) {
        stack_destroy(bg->game[i]);
        stack_destroy(bg->players[i]);
    }
    free(bg->game);
    bg->game = NULL;
    free(bg->players);
    bg->players = NULL;
    free(bg);
}

bool battle_game_compute_playing_id(BattleGame *bg) {
    int check[MAX_VALUE + 1] = {0};
    int *players_equals = calloc(bg->nb_players, sizeof(int));
    int players_added = 0;
    for (int i = 0; i < bg->nb_playing; i++) {
        PlayerId id = bg->playing_id[i] - 1;
        Stack *current = bg->game[id];
        Card * top = stack_read_first(current);
        if (!top)
            continue;
        int value = top->value;
        if (check[value] == 0) {
            check[value] = id + 1;
        } else if (players_added == 0) {
            players_equals[players_added++] = check[value];
            players_equals[players_added++] = id + 1;
        } else {
            players_equals[players_added++] = id + 1;
        }
    }
    if(!players_added){
        free(players_equals);
        return false;
    } else {
        free(bg->playing_id);
        bg->playing_id = NULL;
        bg->playing_id = players_equals;
        bg->nb_playing = players_added;
        return true;
    }
}

bool battle_game_is_ended(BattleGame *bg){
    for(int i = 0; i < bg->nb_players; i++){
        Stack * player_stack = bg->players[i];
        if(player_stack->size == STACK_MAX_SIZE) return true;
    }
    return false;
}
void battle_game_play_card(BattleGame *bg, bool visible){
    for (int i = 0; i < bg->nb_playing; i++) {
        PlayerId id = bg->playing_id[i] - 1;
        if (stack_is_empty(bg->players[id]))
            continue;
        Card *to_play = stack_pop(bg->players[id]);
        to_play->visible = visible;
        stack_add_card_on(bg->game[id], to_play);
        //stack_print(bg->game[i]);
    }
}
PlayerId battle_game_get_winning_player(BattleGame *bg) {
    int max = 0;
    PlayerId id_max = 0;
    for (int i = 0; i < bg->nb_playing; i++) {
        PlayerId id = bg->playing_id[i] - 1;
        Card *top = stack_read_first(bg->game[id]);
        if (!top)
            continue;
        if(top->value > max){
            max = top->value;
            id_max = id;
        }
    }
    return id_max;
}
void battle_game_collect_cards_from_game(BattleGame *bg, PlayerId winner_id) {
    for (int i = 0; i < bg->nb_playing; i++) {
        PlayerId id = bg->playing_id[i] - 1;
        stack_add_stack_on_bottom(bg->players[winner_id], bg->game[id], false);
    }
}
void battle_game_print_game(BattleGame *bg){
    for(int i = 0; i < bg->nb_players; i++){
        stack_print(bg->game[i]);
        printf(" | ");
        stack_print(bg->players[i]);
        printf("\n");
    }
    printf("------------\n");
}
void battle_game_reset_playing_id(BattleGame *bg) {
    free(bg->playing_id);
    bg->playing_id = NULL;
    bg->playing_id = malloc(sizeof(PlayerId) * bg->nb_players);
    bg->nb_playing = 0;
    for (int i = 0; i < bg->nb_players; i++) {
        if (!stack_is_empty(bg->players[i])) {
            bg->playing_id[bg->nb_playing++] = i + 1;
        }
    }
}
void battle_game_play_battle(BattleGame *bg, bool battle) {
    if (!battle) {
        int wp = battle_game_get_winning_player(bg);
        battle_game_reset_playing_id(bg);
        battle_game_print_game(bg);
        battle_game_collect_cards_from_game(bg, wp);
        return;
    }
    printf("Battle\n");
    battle_game_play_card(bg, false);
    battle_game_play_card(bg, true);
    battle = battle_game_compute_playing_id(bg);
    battle_game_play_battle(bg, battle);
}

void battle_game_main_loop(BattleGame *bg){
    int wp = 0;
    bool battle = false;
    while(!battle_game_is_ended(bg)){

        battle_game_play_card(bg, true);
        battle = battle_game_compute_playing_id(bg);

        if (!battle) {
            wp = battle_game_get_winning_player(bg);
            battle_game_print_game(bg);
            battle_game_collect_cards_from_game(bg, wp);
        } else {
            battle_game_play_battle(bg, battle);
        }
    }
}

// TODO :
//  - When a player doesnt have any card in a battle;
//  - When 3 players have a battle, the collect doesn't work properly
//  - Detect a cycle for equality

int main(){
    srand(time(NULL));
    int nb_players = 3;
    BattleGame *bg = battle_game_new(nb_players);
    battle_game_init(bg);
    battle_game_main_loop(bg);
    battle_game_destroy(bg);
    return 0;
}