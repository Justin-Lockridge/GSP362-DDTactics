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
	std::vector<Items> inventory;
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
	Character* getCharacter(int iter){return &army[iter];}
	void setCharacterLevel(int iter, int job, int level);
	void addToInventory(int itemType){inventory[itemType].itemCount += 1;}
	void removeFromInventory(int itemType){if(inventory[itemType].itemCount > 1)inventory[itemType].itemCount -= 1;}
	bool checkInventoryForItem(int itemType){if(inventory[itemType].itemCount > 1) return true;}
	int getMoney();
	void adjustMoney(int amount){money += amount;}
	Inventory getInventory();
	
	void setActiveJob(int iter, int job){army[iter].setCurrentJob(job);}
	void resetStats(int iter, job_mods jobMod);
};