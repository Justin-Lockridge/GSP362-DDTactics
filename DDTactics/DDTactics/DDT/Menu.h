#pragma once

#include "Button.h"
#include "Cursor.h"
#include "SoundManager.h"
//#include "GraphicsManager2D.h"
//#include "InputManager.h"
#include "Cursor.h"
//#include "GameEnums.h"
#include <vector>
#include "MiscStructs.h"

enum MENU_STATE
{
	MENU_MAIN,
	MENU_SAVE,
	MENU_LOAD
};

enum PLAYERSAVEDATA
{
	CURRENTJOB,
	WARRIORLEVEL,
	ROGUELEVEL,
	GREYMAGELEVEL,
	CURRENTEXPERIENCE,
	CURRENTGOLD, 
	
};

#define MAX_BUTTONS 4



class Menu
{
private:
	bool	m_logo;
	float	m_count;
	int		menuState;
	int		menuSelection;	

	std::vector<Button> m_buttons;

	Menu();

public:
	static Menu* instance();
	~Menu();

	void Init();
	void Update(Cursor *cursor, SoundManager *SManager, InputManager *IManager, int &game_state, float dt);
	void Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt);

	

};