#include "game.h"

#include <fstream>
#include <iostream>

#include "game_ui.h"

#include "actors/enemies/acolyte.h"

#include "logger/global_logger.h"
#include "logger/fstream_wrapper.h"

#include "input/key_bind_manager.h"

#include "actors/items/armor_item.h"
#include "actors/items/health_item.h"
#include "actors/items/weapon_item.h"


static void clear_tscreen()
{
	std::cout << "\033[H\033[J";
}



std::vector<actor_spawner_func> game::enemy_spawn_funcs;


game::game(size_t difficulty, game_condition& win_cond, game_condition& lose_cond)
	: difficulty(difficulty), win_cond(win_cond), lose_cond(lose_cond) {}


void game::init()
{
	mainfield = new field(30, 30);
	mainfield_view = new field_view(*mainfield);

	cell_entrance* ce = mainfield->generate_entrance_exit(difficulty * 8);

	mainplr = new player();
	{
		mainplr->setMaxHealth(30);
		mainplr->setHealth(30 / ((double)difficulty / 2));
		mainplr->setTeamID(10);
		mainplr->setMaxAP(8);
		mainplr->setAP(8);
		ce->set_object(mainplr);

		mainplr->addItemToInventory(new health_item(30));
		mainplr->addItemToInventory(new weapon_item(14, 1.));
		mainplr->addItemToInventory(new armor_item(2, 0.2));
	}

	{
		enemy_spawn_funcs.push_back([](const cell& pc){return new acolyte();});
		actor_spawner<enemy_spawn_funcs> enemy_spawner;
		const std::vector<actor*>& enemies_spawned = enemy_spawner.doSpawn(*mainfield, difficulty);
		actors_spawned.insert(actors_spawned.end(), enemies_spawned.begin(), enemies_spawned.end());
	}
}


void game::run()
{
	std::ofstream fd_out;
	fstream_wrapper fdw_out(fd_out, "game_out");
	ostream_wrapper obs_bufw(obs_buf);

	global_logger::init_loggers({fdw_out, obs_bufw});
	observer_actor mainplr_obs({fdw_out, obs_bufw});
	mainplr_obs.setActor(*mainplr);

	std::vector<observer_actor*> actors_obs;
	for(auto i = actors_spawned.begin(); i != actors_spawned.end(); ++i)
	{
		observer_actor* obsa = new observer_actor({fdw_out, obs_bufw});
		obsa->setActor(**i);
		actors_obs.push_back(obsa);
	}

	input_manager inmngr;
	game_ui action_ui(*this, 0, &mainplr->getMoveAction());

	refresh(action_ui);

	int first_iter = 1;
	while(1){
		clear_tscreen();
		if(!first_iter)
			mainfield->tickActors();
		else
			first_iter = 0;

		if(win_cond.isMet()){
			std::cout << "You won!\n";
			break;
		}
		if(lose_cond.isMet()){
			std::cout << "You lost!\n";
			break;
		}

		mainfield_view->display();

		std::cout << obs_buf.str();
		std::cout << '\n';
		action_ui.draw_player_actions();
		std::cout << '\n';

		action_ui.handle_action_menu(inmngr);

		obs_buf.str("");
	}

	for(auto i = actors_obs.begin(); i != actors_obs.end(); ++i)
		delete *i;
}


void game::refresh(game_ui& ui)
{
	clear_tscreen();
	mainfield_view->display();
	std::cout << obs_buf.str();
	std::cout << '\n';
	ui.draw_player_actions();
	std::cout << '\n';
}

game::~game()
{
	delete mainplr;
	for(size_t i = 0; i < actors_spawned.size(); ++i)
		delete actors_spawned[i];
}


// public getters

player& game::getMainPlayer() { return *mainplr; }
field& game::getMainField() { return *mainfield; }
field_view& game::getMainFieldView() { return *mainfield_view; }
