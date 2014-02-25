#pragma once

#include "InputManager.h"
#include "Cursor.h"
#include "Player.h"
#include "GraphicsManager2D.h"
#include "SoundManager.h"
#include "Button.h"
#include <vector>
#include "GameEnums.h"

#define MAX_STATUS_BUTTONS 7

class StatusMenu
{
private:
	StatusMenu();

	std::vector<Button> status_menu_buttons;
	std::vector<Button> status_menu_borders;

	std::vector<Button> status_item_buttons;
	std::vector<Button> status_ability_buttons;
	std::vector<Button> status_magic_buttons;
	std::vector<Button> status_status_buttons;
	
	int status_state;
	
public:
	~StatusMenu();
	static StatusMenu* instance();

	void init();
	void Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt);
	void Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, Player *player, float dt);

};