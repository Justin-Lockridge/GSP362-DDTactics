//////////////////////////////////////////////////////////////////////////////////////
//Written by:	Ricky Rivera														//
//Date:			2/14/2014															//
//Class:		GSP362 DeVry University												//
//Game:			DDTactics															//
//About: So far only has the shop in it for buying and selling						//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SoundManager.h"
#include "Cursor.h"
#include "Button.h"
#include "GameEnums.h"
#include "MiscStructs.h"
#include "Player.h"
#include <queue>

#define MAX_SHOP_SPRITE_POSITIONS 4
#define MAX_SHOP_BUTTONS_POSITIONS 5
#define MAX_WEAPONS_BUTTONS_POTITIONS 4
#define MAX_HELM__BUTTONS_POSITIONS 1
#define MAX_CHEST_BUTTONS_POSITIONS 1
#define MAX_ACCESORY_BUTTONS_POSITIONS 1
#define MAX_POTIONS_BUTTONS_POSITION 5

class Town
{
public:
	int shop_state;

	std::vector<Button> shop_sprite_pos;
	std::vector<Button> shop_mini_buttons_pos;
	std::vector<Button> weapons_buttons_pos;
	std::vector<Button> helm_button_pos;
	std::vector<Button> chest_button_pos;
	std::vector<Button> accesory_button_pos;
	std::vector<Button> potion_button_pos;

	bool main;
	bool weapons;
	bool chest; 
	bool helm; 
	bool accesory; 
	bool potions;
	bool buyMenu;
	bool noMoney;

	float count;

private:
	Town();
public:
	~Town();
	static Town* instance();
	void init();

	void render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt, Player *player, HWND *m_hWnd, ID3DXFont *D3DFont);
	void renderText(ID3DXFont *D3DFont, Player *player, HWND *m_hWnd, float dt);

	void update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager,  int &game_state,  float dt, Player *player);
};
