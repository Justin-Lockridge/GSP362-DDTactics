#pragma once
#include "GameEnums.h"

#include <vector>

#define MAX_ITEMS 100

class Inventory
{
public:
	struct Items
	{
		int buyPrice;
		int sellPrice;
		int type;
		int armor;
		int damage;
		int addHealth;
		int addMana;
	
	};

	std::vector<Items> myItems[MAX_ITEMS]; 

	Inventory(void);
	~Inventory(void);

	Inventory openInventory();

	void add(Items);
	void use(Items);
};