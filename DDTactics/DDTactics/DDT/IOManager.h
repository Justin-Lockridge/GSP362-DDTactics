#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "Overworld.h"

#define MAXSAVES 3
#define MAXPLAYERDATA 6

class IOManager
{
private:



	IOManager();

	std::vector<Button> saveloadButtons;
	std::string m_savedGame[MAXSAVES];

	int m_gameSave[MAXSAVES][MAXPLAYERDATA];
	int iterator;
	

public:

	~IOManager();

	static IOManager* instance();

	void init();


	void update(InputManager *IManager, Cursor *cursor, int &game_state, float dt);
	void savegame(Player *player, Overworld *overworld);
	void loadgame(Player *player, Overworld *overworld);

	void render(GraphicsManager2D *GManager, ID3DXSprite* spriteObj, float dt);

};