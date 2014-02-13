#pragma once


#include "InputManager.h"
#include "Inventory.h"
#include "MiscStructs.h"
#include "GameEnums.h"

class Player
{
private:
	


public:

	D3DXVECTOR2 overworld_pos;
	Overworld_node *current_Node;
	Overworld_node *destination;
	Overworld_node *previous_Node;


	

};