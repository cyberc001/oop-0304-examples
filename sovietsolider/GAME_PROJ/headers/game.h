#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "field_view.h"
#include "player.h"
#include <termios.h>
#include <unistd.h>
#include "enemy.h"
#include "enemyBoss.h"
#include "enemyMove.h"
#include "enemyStand.h"
#include "objectHeal.h"
#include "objectDamage.h"
#include "objectRange.h"
#include "object.h"


class Game
{
	float health, damage;
	public:
		void onStart();
		void clear_terminal_icanon();
		void set_terminal_icanon();
		void clear_screen();
		void initTermios(int echo);

		void move_player_up(Player& player, Field& field);
		void move_player_down(Player& player, Field& field);
		void move_player_left(Player& player, Field& field);
		void move_player_right(Player& player, Field& field);

		void init_fight(Player& player, Enemy* enemy);
		void move_enemies(std::vector< Enemy* > enemy_container);

		void input_control(char symb, Player& player, Field& field, std::vector<Enemy*> enemy_container);

};

#endif 
