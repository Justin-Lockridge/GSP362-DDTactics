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
	std::vector<Battle_Node*> m_areaHighlight;
	std::vector<Button> m_buttons;
	std::vector<Button> m_actButtons1;
	std::vector<Button> m_actButtonsSkills;
	std::vector<Button> m_actButtonsItems;

	CGraph m_Map;
	Battle_Node * m_3Dcursor;
	Character * m_activeChar;

	int m_charState,
		m_actState,
		m_turnIndex;
	float m_count;
	bool moved, acted;
	Battle(void);
public:
	static Battle* instance();
	~Battle(void);

	void Init(Player *player);
	void Shutdown();

	void Update(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				IDirect3DDevice9 *device);
	void Render3D(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, 
				float dt,IDirect3DDevice9 *device);
	void Render2D(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, 
				float dt,IDirect3DDevice9 *device);

	// pass distance/range to highlight set the nodes to highlight
	void highlightMap(int dist);
	bool checkHighlight(Battle_Node * a_node);
	void Act(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				IDirect3DDevice9 *device);
	void Move(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				IDirect3DDevice9 *device);
	void Wait(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				IDirect3DDevice9 *device);
	//void wait();
};

