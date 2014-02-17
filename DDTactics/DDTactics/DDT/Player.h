#pragma once


#include "InputManager.h"
#include "Inventory.h"
#include "MiscStructs.h"
#include "GameEnums.h"
#include "Character.h"

class Player
{
private:
	std::vector<Character> army;
	Inventory inventory;
	int money;
		
	Player();

public:

	//TODO: switch to private, change Overworld to reflect changes
	D3DXVECTOR2 overworld_pos;
	Overworld_node *current_Node;
	Overworld_node *destination;
	Overworld_node *previous_Node;

	
	~Player();

	static Player* instance();
	
	std::vector<Character>* returnArmy();
	int getMoney();
	Inventory getInventory();
	

};