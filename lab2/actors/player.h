#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "item.h"
#include "../actions/action_move.h"

class player : public actor
{
	public:

	// Properites
	virtual char getDisplay(cell& holder);

	size_t& getAP();

	// Inventory
	bool addItemToInventory(item* i);

	// Events
	virtual void onAdd(cell& holder);

	virtual void onTurn();
	virtual void onDeath();

	virtual void onWalkedIn(actor& by_who);
	virtual void onWalkOn(cell& to_where);


	protected:

	size_t ap, max_ap;

	std::vector<item*> inventory;
	size_t max_inventory_size = 10;

	action_move* mv_action;
};

#endif
