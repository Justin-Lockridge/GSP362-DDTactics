#pragma once

#include "SoundManager.h"
#include "Cursor.h"
#include "Button.h"
#include "GameEnums.h"
#include "MiscStructs.h"
#include "Player.h"
#include "CGraph.h"
#include "GraphicsManager3D.h"
#include <queue>

enum CHAR_STATE
{
	START,
	MOVE,
	ACT,
	WAIT,
	CHECK_STATUS,
};

enum ACT_STATE
{
	INITIAL,
	SKILL,
	ITEMS,
	END,
};

class Battle
{
private:
	std::vector<Character> m_Units;
	std::vector<Battle_Node> m_areaHighlight;
	std::vector<Button> m_buttons;

	CGraph m_Map;
	Battle_Node * m_3Dcursor;
	Character * m_activeChar;

	int m_charState,
		m_actState,
		m_turnIndex;

	Battle(void);
public:
	static Battle* instance();
	~Battle(void);

	void Init(Player *player);
	void Shutdown();

	void Update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt);
	void Render(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, float dt);

};

