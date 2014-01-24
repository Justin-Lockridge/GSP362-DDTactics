#include "Inventory.h"

inventory::inventory(void)
{
}

inventory::~inventory(void)
{
}

void inventory::add(Items itemsList)
{
	switch(itemsList.type)
	{
	case HEAD:
		myItems->push_back(itemsList); //Not sure if this is how we want to add stuff to inventory
		break;
	case CHEST:
		myItems->push_back(itemsList);
		break;
	case WEAPON:
		myItems->push_back(itemsList);
		break;
	case POTIONS:
		myItems->push_back(itemsList);
		break;
	case ETHER:
		myItems->push_back(itemsList);
		break;
	default:
		break;
	};
}

void inventory::use(Items thisItem)
{
	switch(thisItem.type)
	{
	case HEAD:
		// Maybe a message that u cannot use this item at this time?
		break;
	case CHEST:
		// Maybe a message that u cannot use this item at this time?
		break;
	case WEAPON:
		// Maybe a message that u cannot use this item at this time?
		break;
	case POTIONS:
		//use item on selected player

		//player.healthpoints += 50; // regular potion
		//player.healthpoints += 100; //hi potion
		break;
	case ETHER:
		//use item on selected player
		//player.manapoints += 50; // regular ether
		//player.manapoints += 100; // hi potion
		break;
	default:
		break;

	};
}