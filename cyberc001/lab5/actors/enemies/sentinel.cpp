#include "sentinel.h"

sentinel::sentinel()
{
	initView();

	max_ap = 9;
	detection_dist = 45;

	setHealth(30);
	setMaxHealth(30);
	setDamageThreshold(8);
	setDamageResistance(0.3);

	atk_action = new action_shoot_ballistic(3, 3, 18);
	mv_action = new action_move(0.7);
}

// Propetries
const char* sentinel::getName() const { return "Sentinel"; }
