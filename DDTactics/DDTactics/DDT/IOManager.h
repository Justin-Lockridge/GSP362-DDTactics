#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "Overworld.h"

#define MAXSAVES 3
#define MAXPLAYERDATA 6
#define SAVEFILELENGTH 64
#define SAVEMENUBUTTONCOUNT 4

class IOManager
{
private:



	IOManager();

	std::vector<Button> saveloadButtons;
	std::string m_saveString[MAXSAVES];

	
	int iterator;
	std::ofstream savedGameFile;

public:
	int m_gameSave[MAXSAVES][SAVEFILELENGTH];
	~IOManager();

	static IOManager* instance();

	void init();


	void update(InputManager *IManager, Cursor *cursor, Player* player, int &game_state, float dt);
	void savegame(int fileNumber, Player *player, Overworld *overworld);
	void loadgame(int fileNumber, Player *player, Overworld *overworld);
	void loadSaves();

	int* getGameSaves(){return &m_gameSave[0][0];}

	void render(GraphicsManager2D *GManager, ID3DXSprite* spriteObj, float dt);

};