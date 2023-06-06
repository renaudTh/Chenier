#ifndef BATTLE_H
#define BATTLE_H

#include "../stack.h"

typedef struct BattleGame_t {
	int nb_players;
	Stack **players;
	Stack **game;
	PlayerId *playing_id;
	int nb_playing;
} BattleGame;

BattleGame *battle_game_new(int nb_players);
void battle_game_init(BattleGame *bg);
void battle_game_destroy(BattleGame *bg);
bool battle_game_compute_playing_id(BattleGame *bg);
bool battle_game_is_ended(BattleGame *bg);
void battle_game_play_card(BattleGame *bg, bool visible);
PlayerId battle_game_get_winning_player(BattleGame *bg);
void battle_game_collect_cards_from_game(BattleGame *bg, PlayerId winner_id);
void battle_game_print_game(BattleGame *bg);
void battle_game_reset_playing_id(BattleGame *bg);
void battle_game_play_battle(BattleGame *bg, bool battle);

void battle_game_main_loop(BattleGame *bg);
#endif