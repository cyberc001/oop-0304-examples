#include "../headers/game.h"
#include <ctype.h>
#include <typeinfo>
#include <vector>
#include "../headers/logger.h"
#include "../headers/fstreamWrapper.h"
#define NUM_OF_ENEMIES 3

void Game::clear_terminal_icanon()
{
	static struct termios told, tnew;
	tcgetattr(STDIN_FILENO, &told);
	tnew = told;
	tnew.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &tnew);
}

void Game::set_terminal_icanon()
{
	static struct termios told, tnew;
	tcgetattr(STDIN_FILENO, &told);
	tnew = told;
	tnew.c_lflag |= ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &tnew);
}

void Game::input_control(char symb, Player& player, Field& field,
std::vector<Enemy*> enemy_container)
{
	if(symb == 'q')
			exit(1);
		else if(symb == 'w')
			player.move_up(field);
		else if(symb == 's')
			player.move_down(field);
		else if(symb == 'a')
			player.move_left(field);
		else if(symb == 'd')
			player.move_right(field);
		else if(symb == 'f')
		{
			for(int i=0; i<enemy_container.size(); i++)
				player.init_fight(enemy_container.at(i), field);
		}
}

void Game::clear_screen()
{
	std::cout << "\033[H\033[J";
}


void Game::onStart()
{
	std::ofstream file; /// без обертки
	FstreamWrapper file_w(file, "gameOut");
	OstreamWrapper cout_w(std::cout);
	Logger::initGlobal({file_w, cout_w});
	//std::vector<std::reference_wrapper<OstreamWrapper>> streams;
	//main logic
	clear_terminal_icanon();
	int input;
	Field field(20, 20); Player player;
	std::vector< Enemy* > enemy_container;
	std::vector< Object* > objects_container;
	enemy_container.push_back(new EnemyBoss);          
	enemy_container.push_back(new EnemyStand);
	enemy_container.push_back(new EnemyMove);
	objects_container.push_back(new ObjectHeal);
	objects_container.push_back(new ObjectDamage);
	objects_container.push_back(new ObjectRange);

	
	//if(enemy_container.size()*6+objects_container.size() > field.get_size_x()*field.get_size_y())
	//	return;

	//if(enemy_container.size())

	Cell& entrance = field.put_exits(2);  //ссылка на вход
	field.put_enemies(enemy_container);
	field.put_object(objects_container);

	player.set_posx(entrance.get_x()); player.set_posy(entrance.get_y()); //coords in player according to entrance
	field.get_cell(player.get_posx(), player.get_posy()).set_display('3');
	entrance.set_player(player);
	
	FieldView print(field);


	//clear_screen();
	print.display();
	input = fgetc(stdin);
	input_control(input, player, field, enemy_container);

	clear_screen();
	while(true)
	{
		clear_screen();

		print.display();
		if(player.check_for_death(field))
			return;
		if(field.get_cell(player.get_posx(), player.get_posy()).get_exit())
			return;
		//std::cout << player.get_health() << std::endl;
		
		for(int i=0; i<objects_container.size();i++)
		{
			objects_container.at(i)->onPass(player, field);
			objects_container.at(i)->check_for_death(field, objects_container, i);
		}
		for(int i=0;i<enemy_container.size();i++)
		{
			enemy_container.at(i)->init_fight(player, field);
			enemy_container.at(i)->move(player, field);
			enemy_container.at(i)->check_for_death(field, enemy_container, i);			
		}
		Logger::getGlobal().display("");
		input = fgetc(stdin);
		input_control(input, player, field, enemy_container);


	}
	set_terminal_icanon();
	
}
