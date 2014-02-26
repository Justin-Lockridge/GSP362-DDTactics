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

enum STATUSABILITY
{
	STATUSABILITY_USE,
	STATUSABILITY_ORGANIZE,
	STATUSABILITY_BACK,

};

enum STATUSMAGIC
{
	STATUSMAGIC_USE,
	STATUSMAGIC_ORGANIZE,
	STATUSMAGIC_BACK,
};

class StatusMenu
{
private:
	StatusMenu();

	std::vector<Button> status_menu_buttons;
	std::vector<Button> status_menu_borders;

	std::vector<Button> status_menu_arrow_button;


	std::vector<Button> status_item_buttons;
	
	std::vector<Button> status_job_arrows;
	std::vector<Button> status_job_buttons;

	std::vector<Button> status_status_buttons;

	std::vector<Character*> drawChar;
	std::vector<Character*> copyChar;
	std::vector<D3DXVECTOR3> screenPos;
	int playerCharacters;

	Character* tempChar;

		
	int status_state;

	wchar_t word[64];
	int     num;

	
	
public:
	~StatusMenu();
	static StatusMenu* instance();

	void init(Player *player);
	void Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt);
	void Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, Player *player, float dt);

	void drawText(ID3DXFont *font, Player * player);

	void recreateList(Player* player);

	void shutdown();

	void PushDemButtons(RData buttons[], unsigned int size, std::vector<Button> &container);

};