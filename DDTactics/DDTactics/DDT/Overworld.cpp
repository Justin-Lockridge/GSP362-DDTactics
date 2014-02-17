#include "Overworld.h"

Overworld::Overworld()
{
	overworld_state = OVERWORLD_SELECTION;
	town_state = TOWN_SELECT;
	menu_sprite_pos.clear();

}

Overworld::~Overworld()
{}

Overworld* Overworld::instance()
{
	static Overworld overworld;
	return &overworld;
}

void Overworld::init(Player *player)
{
	Area_location temp_node_pos[] =
	{
		{300.0f, 250.0f},
		{400.0f, 350.0f},
		{200.0f,400.0f},

	};


	Overworld_node tempMap[MAX_AREAS];

	//initialize node positions
	for(int i = 0; i < MAX_AREAS; i++)
	{
		tempMap[i].node_position.x = temp_node_pos[i].x;
		tempMap[i].node_position.y = temp_node_pos[i].y;
		map.push_back(tempMap[i]);
	}

	simpleRData NodeLinks[] =
	{
		//Used to link to other nodes.  If a direction is not linked to another node, link the node to itself
		//up, down, left, right
		{0,1,0,0},
		{0,1,2,1},
		{2,2,2,1}
	};

	//link map nodes
	for(int i = 0; i < MAX_AREAS; i++)
	{
		map[i].up = &map[NodeLinks[i].t];
		map[i].down = &map[NodeLinks[i].b];
		map[i].left = &map[NodeLinks[i].l];
		map[i].right = &map[NodeLinks[i].r];

	}

	int temp_node_type[] =
	{
		0, 1, 2,

	};

	for(int i = 0; i < MAX_AREAS; i++)
	{
		map[i].area_num = temp_node_type[i];
		map[i].area_type = temp_node_type[i];
		switch(map[i].area_type)
		{
		case TYPE_TOWN:
			map[i].node_color = D3DCOLOR_ARGB(175, 128, 128, 255);
			break;
		case TYPE_STORY:
			map[i].node_color = D3DCOLOR_ARGB(175, 255,128,128);
			break;
		case TYPE_RANDOM_BATTLE:
			map[i].node_color = D3DCOLOR_ARGB(175, 128,255,128);
			break;
		}
	}


	simpleRData temp_char_sprite_pos[] =
	{
		{0, 90, 90, 140 },
		{0, 90, 140, 190},
		{0, 90, 190, 250},

	};

	//initialize Rect data for character sprite
	for(int i = 0; i < MAX_CHAR_SPRITE_POSITIONS; i++)
	{
		char_sprite_pos[i].bottom = temp_char_sprite_pos[i].b;
		char_sprite_pos[i].left = temp_char_sprite_pos[i].l;
		char_sprite_pos[i].right = temp_char_sprite_pos[i].r;
		char_sprite_pos[i].top = temp_char_sprite_pos[i].t;
	}


	RData temp_menu_pos[] = 
	{
		{100, 0, 150, 200, 710, 80, false },
		{175, 0, 225, 200, 710, 120, false},
		{240, 0, 280, 200, 710, 160, false},
		{300, 0, 360, 200, 710, 200, false},
	};

	for(int i = 0; i < MAX_MENU_SPRITE_POSITIONS; i++)
	{
		Button temp;
		RECT rect;

		rect.top = temp_menu_pos[i].t;
		rect.left = temp_menu_pos[i].l;
		rect.bottom = temp_menu_pos[i].b;
		rect.right = temp_menu_pos[i].r;

		temp.setPos(temp_menu_pos[i].x, temp_menu_pos[i].y);
		temp.setHighlight(temp_menu_pos[i].highlight);
		temp.setRect(rect);

		menu_sprite_pos.push_back(temp);		

	}


	screen_pos.x = -400.0f;
	screen_pos.y = 360.0f;

	count = 0.0f;
	count2 = 0;
	count3 = 0.0f;
	count4 = 0.0f;
	face = 1.0f;

	menu_pos.x = 700.0f;
	menu_pos.y = 100.0f;

	player->overworld_pos = map[0].node_position;
	player->current_Node = &map[0];
	player->previous_Node = &map[0];
	player->destination = 0;
	


}


void Overworld::render(GraphicsManager2D* GManager, ID3DXSprite *spriteObj, D3DXVECTOR2 &playerOverworldPos, float dt)
{

	//draw the map
	GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 1.0f), 
		D3DXVECTOR3(screen_pos.x, screen_pos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		spriteObj, 
		GRAPHICS_OVERWORLD, 
		D3DCOLOR_ARGB(255,255,255,255));

	//Draw the nodes
	for(int i = 0; i < MAX_AREAS; i++)
	{
		int temp = 0;

		switch(map[i].area_type)
		{
		case TYPE_TOWN:
			temp = GRAPHICS_OVERWORLD_TOWN;
			break;
		case TYPE_STORY:
			temp = GRAPHICS_OVERWORLD_STORY;
			break;
		case TYPE_RANDOM_BATTLE:
			temp = GRAPHICS_OVERWORLD_BATTLE;
			break;
		}

		GManager->Draw2DObject(D3DXVECTOR3(0.75f, 0.75f, 0.0f), 
			D3DXVECTOR3(map[i].node_position.x, map[i].node_position.y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			spriteObj, 
			temp,  
			map[i].node_color);
	}

	//Draw the player
	count += dt;

	if(count > 0.3f)
	{
		count = 0.0f;
		count2++;
		if(count2 >= 3)
		{
			count2 = 0;
		}
	}


	GManager->DrawButton(D3DXVECTOR3(face, 1.0f, 1.0f), 
		D3DXVECTOR3(playerOverworldPos.x, playerOverworldPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), char_sprite_pos[count2], 
		spriteObj, 
		GRAPHICS_OVERWORLD_CHARACTER, 
		(char_sprite_pos[count2].right - char_sprite_pos[count2].left), 
		(char_sprite_pos[count2].bottom - char_sprite_pos[count2].top),
		D3DCOLOR_ARGB(255,255,255,255));


	//Draw the Menu
	GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.0f), 
		D3DXVECTOR3(menu_pos.x, menu_pos.y, 0.0f), 
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		spriteObj, 
		GRAPHICS_OVERWORLD_MENU_BACKGROUND, 
		D3DCOLOR_ARGB(128, 255, 255, 255));

	//Draw the font
	for(unsigned int i = 0; i < menu_sprite_pos.size(); i++)
	{
		GManager->DrawButton(D3DXVECTOR3(0.5f, 0.5f, 0.0f), 
			D3DXVECTOR3(menu_sprite_pos[i].getPos().x, menu_sprite_pos[i].getPos().y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), menu_sprite_pos[i].getRect(), 
			spriteObj, 
			GRAPHICS_OVERWORLD_MENU_TEXT, 
			menu_sprite_pos[i].width,
			menu_sprite_pos[i].height,
			menu_sprite_pos[i].getColor());
	}



}


void Overworld::update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt)
{
	SManager->playStream(SONG_OVERWORLD);
	switch(overworld_state)
	{
	case OVERWORLD_SELECTION:
		{
			//Temporary storage for positions
			float temp[MAX_AREAS];

			//Load X positions into temporary storage
			for(int i = 0; i < MAX_AREAS; i++)
				temp[i]  = map[i].node_position.x;

			//Scroll the screen in the X direction
			screenscroll(cursorPos.x, 790, screen_pos.x, player->overworld_pos.x, -2, -600, temp, dt);
			screenscroll2(cursorPos.x, 10, screen_pos.x, player->overworld_pos.x, 2, 640, temp, dt);

			//Assign new X positions 
			for(int i = 0; i < MAX_AREAS; i++)
				map[i].node_position.x = temp[i];

			//Load the Y positions into temporary storage
			for(int i = 0; i < MAX_AREAS; i++)
				temp[i] = map[i].node_position.y;

			//Scroll the screen in the Y direction
			screenscroll(cursorPos.y, 590, screen_pos.y, player->overworld_pos.y, -2, -50, temp, dt);
			screenscroll2(cursorPos.y, 10, screen_pos.y, player->overworld_pos.y, 2, 360, temp, dt);

			//Assign new Y positions
			for(int i = 0; i < MAX_AREAS; i++)
				map[i].node_position.y = temp[i];

			//Check for cursor over Overworld menu 
			for(int i = 0; i < MAX_MENU_SPRITE_POSITIONS; i++)
			{
				if(menu_sprite_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
				{
					menu_sprite_pos[i].setHighlight(true);
					menu_sprite_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
				}else
				{
					menu_sprite_pos[i].setHighlight(false);
					menu_sprite_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				//Check for cursor over menu option and left mouse button click
				if(menu_sprite_pos[i].isHighlighted() && IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
				{
					//Switch states accordingly
					switch(i)
					{
					case 0:
						overworld_state = OVERWORLD_MENU;
						break;
					case 1:
						overworld_state = OVERWORLD_OPTION;
						break;
					case 2:
						overworld_state = OVERWORLD_SAVE;
						break;
					case 3:
						overworld_state = OVERWORLD_LOAD;
						break;
					}
				}
			}


			//Check for cursor over Node areas
			for(int i = 0; i < MAX_AREAS; i++)
			{
				//Check mouseover
				if(isOn_Node(map[i], cursorPos))
				{
					switch(map[i].area_type)
					{
					case TYPE_TOWN:
						map[i].node_color = D3DCOLOR_ARGB(175, 192, 192, 255);
						break;
					case TYPE_STORY:
						map[i].node_color = D3DCOLOR_ARGB(175, 255,192,192);
						break;
					case TYPE_RANDOM_BATTLE:
						map[i].node_color = D3DCOLOR_ARGB(175, 192,255,192);
						break;
					}
				}else
				{
					switch(map[i].area_type)
					{
					case TYPE_TOWN:
						map[i].node_color = D3DCOLOR_ARGB(175, 128, 128, 255);
						break;
					case TYPE_STORY:
						map[i].node_color = D3DCOLOR_ARGB(175, 255,128,128);
						break;
					case TYPE_RANDOM_BATTLE:
						map[i].node_color = D3DCOLOR_ARGB(175, 128,255,128);
						break;
					}
				}

				if(isOn_Node(map[i], cursorPos) && IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
				{

					overworld_state = OVERWORLD_CHECK;
					player->destination = &map[i];
				}
			}




			break;
		}

	case OVERWORLD_CHECK:
		if( (player->current_Node == player->destination) && (player->current_Node->area_type == TYPE_STORY)
			)
		{
			overworld_state = OVERWORLD_BATTLE;
			player->current_Node->area_type = TYPE_RANDOM_BATTLE;
			break;
		}


		//Check if current node is the destination, and if it's a town node, switch states
		if( (player->current_Node == player->destination) && (player->current_Node->area_type == TYPE_TOWN)
			
			)
		{
			overworld_state = OVERWORLD_TOWN;
			break;
		}


		
		//Check if current node is the same as destination and the node is not a town
		else if(player->current_Node == player->destination)
		{
			overworld_state = OVERWORLD_SELECTION;
			break;
		}


		//Check current position for possible paths to next node and load into path queue
		else
		{
			//ensure the previous node and current node are the same
			//player.previous_Node = player.current_Node;
			//loadPath(player.current_Node, player.previous_Node, player.destination);
			
			overworld_state = OVERWORLD_TRANSITION;

			break;
		}
		break;
	case OVERWORLD_TRANSITION:
		//Move player
		
		//player.previous_Node = player.current_Node;
		//player.current_Node = player.destination;		
		//player.overworld_pos = player.current_Node->node_position;

		count4 += dt;
		if(count4 > 0.05f)
		{
			count4 = 0.0f;
			if(player->overworld_pos.x > player->destination->node_position.x)
			{
				player->overworld_pos.x -= 1.0f;
				face = 1.0f;
			}
			if(player->overworld_pos.x < player->destination->node_position.x)
			{
				player->overworld_pos.x += 1.0f;
				face = -1.0f;
			}
			if(player->overworld_pos.y > player->destination->node_position.y)
				player->overworld_pos.y -= 1.0f;
			if(player->overworld_pos.y < player->destination->node_position.y)
				player->overworld_pos.y += 1.0f;
		}
		//Once player reaches destination
		if(player->overworld_pos.x == player->destination->node_position.x &&
			player->overworld_pos.y == player->destination->node_position.y)
		{
			player->current_Node = player->destination;
			overworld_state = OVERWORLD_SELECTION;
		}
		//Clear out vector and queue
		//potential_path.clear();
		//path = std::queue<Overworld_node*>();

		//If random battle occurs
	//	overworld_state = OVERWORLD_BATTLE;

		break;
	case OVERWORLD_TOWN:
		SManager->stopStream();
		overworld_state = OVERWORLD_SELECTION;
		game_state = TOWN;
		break;
		//TODO: move to TOWN class
		/*switch(town_state)
		{
		case TOWN_SELECT:
		break;
		case TOWN_SHOP:
		break;
		case TOWN_RECRUIT:
		break;
		case TOWN_EXIT:
		break;
		}*/

	case OVERWORLD_MENU:
		overworld_state = OVERWORLD_SELECTION;
		game_state = STATUS;
		break;
	case OVERWORLD_OPTION:
		if(IManager->push_button(DIK_BACKSPACE))
			overworld_state = OVERWORLD_SELECTION;
		break;
	case OVERWORLD_BATTLE:
		SManager->stopStream();
		overworld_state = OVERWORLD_SELECTION;
		game_state = BATTLE;
		break;

	case OVERWORLD_SAVE:
		SManager->stopStream();
		overworld_state = OVERWORLD_SELECTION;
		game_state = SAVE;
		break;
	case OVERWORLD_LOAD:
		SManager->stopStream();
		overworld_state = OVERWORLD_SELECTION;
		game_state = LOAD;
		break;


	}
}

void Overworld::screenscroll(float position, float edge, float &screenVal, float &playerPos, int adjust, float outer_edge, float *nodePos, float dt)
{
	count3 += dt;
	if(position >= edge)
	{
		if(count3 >= 0.05f)
		{
			count3 = 0.0f;
			screenVal += adjust;
			playerPos += adjust;

			for(int i = 0; i < MAX_AREAS; i++)
				nodePos[i] += adjust;

			if(screenVal < outer_edge)
			{
				screenVal = outer_edge;
				playerPos += -adjust;
				for(int i = 0; i < MAX_AREAS; i++)
					nodePos[i] += -adjust;
			}
		}
	}
}

void Overworld::screenscroll2(float position, float edge, float &screenVal, float &playerPos, int adjust, float outer_edge, float *nodePos, float dt)
{
	count3 += dt;
	if(position <= edge)
	{
		if(count3 >= 0.05f)
		{
			count3 = 0.0f;
			screenVal += adjust;
			playerPos += adjust;

			for(int i = 0; i < MAX_AREAS; i++)
				nodePos[i] += adjust;

			if(screenVal > outer_edge)
			{
				screenVal = outer_edge;
				playerPos += -adjust;
				for(int i = 0; i < MAX_AREAS; i++)
					nodePos[i] += -adjust;
			}
		}
	}
}

std::vector<Overworld_node> Overworld::getMapData()
{
	return map;
}

void Overworld::setMapData(int* areaType, bool* storybattle)
{
	for(unsigned int i = 0; i < map.size(); i++)
	{
		map[i].area_type = areaType[i];
		map[i].story_battle = storybattle[i];

	}
}

Overworld_node Overworld::getNode(unsigned int ENUM_VAL)
{

	return map[ENUM_VAL];
}

bool Overworld::isOn_Node(Overworld_node &node, D3DXVECTOR2 &cursorPos)
{
	return( cursorPos.x < (node.node_position.x + (node.width / 2)) &&
		cursorPos.x > (node.node_position.x - (node.width / 2)) &&
		cursorPos.y < (node.node_position.y + (node.height / 2)) &&
		cursorPos.y > (node.node_position.y - (node.height / 2))		
		);
}

bool Overworld::loadPath(Overworld_node *start, Overworld_node *previous, Overworld_node *destination)
{
	
		if(start == destination)
		{
			path.push(start);
			return true;
		}
		if(start != previous)
			potential_path.push_back(start);
		

		loadPath(start->up, start, destination);
		loadPath(start->down, start, destination);
		loadPath(start->left, start, destination);
		loadPath(start->right, start, destination);

		//Check if any of start's directions are the destination
		if(start->up == destination)
			potential_path.push_back(start->up);
		if(start->down == destination)
			potential_path.push_back(start->down);
		if(start->left == destination)
			potential_path.push_back(start->left);
		if(start->right == destination)
			potential_path.push_back(start->right);

		if(potential_path.back() == destination)
			path.push(potential_path.front());

		//if the destination isn't found, load paths that point to other nodes
		if(potential_path.empty())
		{
			if(start->up != start)
				potential_path.push_back(start->up);
			if(start->down != start)
				potential_path.push_back(start->down);
			if(start->left != start)
				potential_path.push_back(start->left);
			if(start->right != start)
				potential_path.push_back(start->right);

		}

		

		return false;
}