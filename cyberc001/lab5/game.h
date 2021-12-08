#ifndef GAME_H
#define GAME_H

#include <sstream>

#include "logger/ostream_wrapper.h"
#include "logger/observer_actor.h"

#include "rule/actor_spawner.h"
#include "rule/actor_spawner.cpp"
#include "rule/game_condition.h"

#include "actors/player.h"

#include "input/input_manager.h"

#include "field.h"
#include "field_view.h"

class game_ui;

class game
{
	private:

	field* mainfield;
	field_view* mainfield_view;
	std::stringstream obs_buf; // буффер для событий изменения сущностей

	player* mainplr = nullptr;

	static std::vector<actor_spawner_func> enemy_spawn_funcs;
	std::vector<actor*> actors_spawned;

	// game parameters
	size_t difficulty;
	game_condition& win_cond;
	game_condition& lose_cond;


	public:

	// game management
	game(size_t difficulty, game_condition& win_cond, game_condition& lose_cond);
	~game();

	void init();
	void run();


	// UI
	void refresh(game_ui& ui);

	// public getters
	player& getMainPlayer();
	field& getMainField();
	field_view& getMainFieldView();
};

#endif
