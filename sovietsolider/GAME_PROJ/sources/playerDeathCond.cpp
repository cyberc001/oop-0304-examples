#include "../headers/playerDeathCond.h"

template<int death_health>
bool DeathCond<death_health>::isDone()
{
	return player->get_health() <= 0;
}

template<int death_health>
void DeathCond<death_health>::setPlayer(Player& player)
{
	this->player = &player;
}