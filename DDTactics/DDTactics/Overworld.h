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

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

#include "Button.h"


#define MAX_AREAS 3

enum OVERWORLD_STATES{
	OVERWORLD_SELECTION,
	OVERWORLD_TRANSITION,
	OVERWORLD_TOWN,
	OVERWORLD_SHOP,
	OVERWORLD_MENU,
	OVERWORLD_STATUS,
	OVERWORLD_BATTLE,
	OVERWORLD_SAVE,
	OVERWORLD_LOAD
};

//enum OVERWORLD_RENDER_STATES{
//	O_RENDER_MAP,
//	O_RENDER_TOWN,
//	O_RENDER_SHOP
//};

enum TOWN_STATES{
	TOWN_SELECT,
	TOWN_SHOP,
	TOWN_RECRUIT,
	TOWN_EXIT
};

enum NODE_TYPES{
	TYPE_TOWN,
	TYPE_STORY,
	TYPE_RANDOM_BATTLE
};

enum NODE_NAMES{

};

struct O_node
{
	D3DXVECTOR2 node_position;	//Position of the node in 2D space
	O_node * up;
	O_node * down;
	O_node * left;
	O_node * right;

	int area_type;

	O_node()
	{
		up = 0;
		down = 0;
		left = 0;
		right = 0;
	}

};

struct O_Player{
	D3DXVECTOR2 pos;				//for transitioning
	O_node *current_node_position;  //Player's current location

};

struct RectData{
	int t, b, l, r;
};

class Overworld
{
private:
	//Internal private struct Node for linking map nodes
	struct Area_location{
		float x, y;
	};
	

	unsigned int		overworld_state;	//Overworld states
	unsigned int		town_state;			//Overworld town states
	O_node				map[MAX_AREAS];
	O_Player			overworld_player;
	D3DXVECTOR2			screen_pos; //view of the overworld map
	D3DXVECTOR2			menu_position;
	int					menu_alpha;

	RECT			char_sprite_position[3];	//positions on sprite sheet
	int	count, count2;

	FMOD::Sound *m_OverworldMusic;
	FMOD::Sound *m_TownMusic;

	
	IDirect3DTexture9	*m_Overworld_Texture, 
						*m_Char_texture, 
						*m_Area_texture,
						*m_menu_background,
						*m_menu_words;

	D3DXIMAGE_INFO		m_Overworld_Info,
						m_O_Char_Info,
						m_O_Area_Info,
						m_menu_background_info,
						m_words_info;
	



public:
	Overworld();					
	~Overworld();					//Default deconstructor - NOT USED - call shutdown instead


	//////////////////////////////////////////////////////////////////////////////////////
	//Name:			init																//
	//Parameters:	IDirect3DDevice9 *device - pointer to 3D Device object				//
	//				FMOD::System *fmodSystem - pointer to the FMOD system object		//
	//Return:		none																//
	//Description:	This function initializes overworld variables.						//
	//////////////////////////////////////////////////////////////////////////////////////
	void init(IDirect3DDevice9 *device, FMOD::System* fmodSystem);

	//////////////////////////////////////////////////////////////////////////////////////
	//Name:			render																//
	//Parameters:																		//
	//				ID3DXSprite *drawSpriteObj - pointer to the sprite object			//
	//				IDirect3DTexture9* Overworldmap - pointer to the map texture		//
	//				IDirect3DTexture9* Character - pointer to the character texture		//
	//				D3DXIMAGE_INFO &map_info - address of the map image information		//
	//				D3DXIMAGE_INFO &char_info - address of the character image			//
	//											information								//

	void render( ID3DXSprite *drawSpriteObj, IDirect3DTexture9 *cursor, D3DXIMAGE_INFO &cursor_info,
				D3DVECTOR &cursor_pos, float dt);

	void update(int GAMESTATEBATTLE, int &game_state, DIMOUSESTATE2 &a_mouseState, 
				char buffer[255], FMOD::System* fmodSystem, FMOD::Channel* m_musicChannel, 
				D3DVECTOR &cursor_pos);

	void shutdown();



};