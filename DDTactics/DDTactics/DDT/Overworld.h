//////////////////////////////////////////////////////////////////////////////////////
//Written by:	Smyth Chang															//
//Date:			1/18/2014															//
//Class:		GSP362 DeVry University												//
//Game:			DDTactics															//
//About:																			//
//				The Overworld class encapsulates all features and functionalities	//
//				that have to do with the overworld map.								//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SoundManager.h"
#include "Cursor.h"
#include "Button.h"
#include "GameEnums.h"
#include "MiscStructs.h"
#include "Player.h"
#include <queue>



#define MAX_AREAS 3
#define MAX_CHAR_SPRITE_POSITIONS 3
#define MAX_MENU_SPRITE_POSITIONS 4

class Overworld
{
private:
	unsigned int overworld_state;
	unsigned int town_state;

	//Overworld_node map[MAX_AREAS];
	std::vector<Overworld_node> map;
	std::vector<Button> map_buttons;
	
	std::queue<Overworld_node*> path;
	std::vector<Overworld_node*> potential_path;

	D3DXVECTOR2		screen_pos;
	D3DXVECTOR2		menu_pos;

	RECT char_sprite_pos[MAX_CHAR_SPRITE_POSITIONS];
	std::vector<Button> menu_sprite_pos;

	float count, count3, count4;
	int count2;
	float face;

	

private:
	Overworld();

public:
	~Overworld();

	static Overworld* instance();


	//////////////////////////////////////////////////////////////////////////////////////
	//Name:			init																//
	//Parameters:	D3DXVECTOR2 player_O_pos - the player's overworld position			//
	//				Overworld_node *current_Node - used to determine which node the		//
	//								player is on.										//
	//Return:		none																//
	//Description:	This function initializes overworld variables.						//
	//////////////////////////////////////////////////////////////////////////////////////
	void init(Player &player);

	//////////////////////////////////////////////////////////////////////////////////////
	//Name:			render																//
	//Parameters:	GraphicsManager2D *GManager - the graphics manager -  used to draw	//
	//				ID3DXSprite *spriteObj	- the sprite object							//
	//				D3DXVECTOR2 &playerOverworldPos - player's overworld position		//
	//				float dt - elapsed time												//
	//Return:		void																//
	//Description:	draws the overworld map, character, nodes, menu, etc.				//
	//////////////////////////////////////////////////////////////////////////////////////
	void render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, D3DXVECTOR2 &playerOverworldPos, float dt);

	//////////////////////////////////////////////////////////////////////////////////////
	//Name:			update																//
	//Parameters:	D3DXVECTOR2 cursorPos - the position of the cursor					//
	//				InputManager *IManager - the input manager object - used for getting//
	//										 player input								//
	//				SoundManager *SManager - the sound manager object					//
	//				Player &player - the player object									//
	//				float dt - elapsed time												//
	//Return:		void																//
	//Description:	draws the overworld map, character, nodes, menu, etc.				//
	//////////////////////////////////////////////////////////////////////////////////////
	void update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, Player &player, int &game_state,  float dt);

	void screenscroll(float position, float edge, float &screenVal , float &playerPos, int adjust, float outer_edge,  float *nodePos, float dt);
	void screenscroll2(float position, float edge, float &screenVal , float &playerPos, int adjust, float outer_edge,  float *nodePos, float dt);
	
	std::vector<Overworld_node> getMapData();
	void setMapData(int *areaType , bool *storybattle);

	Overworld_node getNode(unsigned int ENUM_VAL);
	bool isOn_Node(Overworld_node &node, D3DXVECTOR2 &cursorPos); 

	bool loadPath(Overworld_node *start, Overworld_node *previous, Overworld_node *destination);
};
