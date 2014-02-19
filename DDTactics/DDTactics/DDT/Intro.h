#pragma once

#include "InputManager.h"
#include "Cursor.h"
#include "Player.h"
#include "GraphicsManager2D.h"
#include "SoundManager.h"
#include "Button.h"
#include <vector>

#define MAX_INTRO_BUTTONS 1

class IntroMenu
{
private:
	IntroMenu();

	std::vector<Button> introMenuButtons;
	std::vector<Button> introMenuBorders;


public:
	~IntroMenu();
	static IntroMenu* instance();

	void init();
	void Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt);
	void Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt);

};