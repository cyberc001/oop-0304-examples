#include "player.h"

#include "../cells/cell_exit.h"

// Properties

char player::getDisplay(cell& holder) { return '@'; }

size_t& player::getAP() { return ap; }


// Inventory

bool player::addItemToInventory(item* i)
{
	for(size_t j = 0; j < inventory.size(); ++j)
		if(inventory[j]->join(*i))
			return true;

	if(inventory.size() >= max_inventory_size)
		return false;

	inventory.push_back(i);
	return true;
}


// Events

void player::onAdd(cell& holder) {}

void player::onTurn()
{
	ap = max_ap;
}
void player::onDeath() {}

void player::onWalkedIn(actor& by_who) {}
void player::onWalkOn(cell& to_where)
{
	cell_exit* ce = dynamic_cast<cell_exit*>(&to_where);

	if(ce){
		
	}
}
