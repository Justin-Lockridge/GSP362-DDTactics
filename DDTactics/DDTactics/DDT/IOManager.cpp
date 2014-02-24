#include "IOManager.h"


IOManager::IOManager()
{}



IOManager::~IOManager()
{
	savedGameFile.close();
}


IOManager* IOManager::instance()
{
	static IOManager iomanager;
	return &iomanager;
}

void IOManager::init()
{
	RData uiData[] =
	{
		{60,  0, 140, 280, 110, 235, false},
		{140, 0, 210, 280, 110, 395, false},
		{215, 0, 285, 280, 110, 555, false},
		{285, 0, 360, 450, 150, 80,  false},
	
	};

	for(int i = 0; i < SAVEMENUBUTTONCOUNT; i++)
	{
		Button temp;
		RECT rect;

		rect.top = uiData[i].t; rect.left = uiData[i].l;
		rect.bottom = uiData[i].b; rect.right = uiData[i].r;
		temp.setPos(uiData[i].x,uiData[i].y);
		temp.setHighlight(uiData[i].highlight);
		temp.setRect(rect);
		temp.setColor(D3DCOLOR_ARGB(255,255,255,255));
		saveloadButtons.push_back(temp);

	}

	///////////////////////////////////////////////////////////
	//  INFO:  Initialize m_savedGame array
	for(int i = 0; i < MAXSAVES; ++i){
		for(int j = 0; j < SAVEFILELENGTH; ++j){
			m_gameSave[i][j] = 0;
		}
	}
	///////////////////////////////////////////////////////////
	//  INFO:  Initializes save strings
	for(int i = 0; i < MAXSAVES; ++i){
		m_saveString[0] = '/0';
	}
	iterator = 0;

}


void IOManager::update(InputManager *IManager, Cursor* cursor, Player* player, int &game_state, float dt)
{
	if(IManager->push_button(DIK_S))
	{
		int temp = 0;
		if(!IManager->check_button_down(DIK_S))
		{
			IManager->set_button(DIK_S, true);
			++iterator;

		}
	}else{
		IManager->set_button(DIK_S, false);
	}

	if(IManager->push_button(DIK_BACKSPACE))
	{
		if(!IManager->check_button_down(DIK_BACKSPACE))
		{
			IManager->set_button(DIK_BACKSPACE, true);
			game_state = MENU;
		}
	}else
		IManager->set_button(DIK_BACKSPACE, false);

	if(IManager->push_button(DIK_RSHIFT))
	{
		if(!IManager->check_button_down(DIK_RSHIFT))
		{
			IManager->set_button(DIK_RSHIFT, true);
			game_state = MENU;
		}
	}else
		IManager->set_button(DIK_RSHIFT, false);
	if(IManager->push_button(DIK_7)){
		if(!IManager->check_button_down(DIK_7)){
			IManager->set_button(DIK_7, true);
			m_gameSave[2][0] = 1;
			m_gameSave[2][2] = 14;
		}
	}else
		IManager->set_button(DIK_7, false);

	for(auto &buttons: saveloadButtons)
	{
		if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 3))
		{

			buttons.setColor(D3DCOLOR_ARGB(255,255,255,0));
			buttons.setHighlight(true);
		}
		else
		{
			buttons.setColor(D3DCOLOR_ARGB(255,255,255,255));
			buttons.setHighlight(false);
		}

	}

	///////////////////////////////////////////////
	//  INFO:  Save / Load game controls
	int selection = -1;
	if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
		//for(auto &buttons:  saveloadButtons){
		for(int i = 0; i < SAVEMENUBUTTONCOUNT; ++i){
			if(saveloadButtons[i].isOn(cursor->cursorPos.x, cursor->cursorPos.y, 3)){
				selection = i;
				break;
			}
		}
	}
	switch(selection){
	case -1:  // Did not select a button
		break;
	case 0:  //  Save 1
		//break;
	case 1:  //  Save 2
		//break;
	case 2:  //  Save 3
		if(game_state == LOAD){
			loadgame(selection, player, 0);
			game_state = OVERWORLD;
		}
		else if (game_state == SAVE)
			savegame(selection, player, 0);
		break;
	case 3: //  Main Menu Button
		game_state = MENU;
		break;
	};
}



void IOManager::render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt)
{
	GManager->Draw2DObject( D3DXVECTOR3(0.4f, 0.6f, 0.0f),
		D3DXVECTOR3(265,235,0.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f),
		spriteObj,
		GRAPHICS_SAVE_BACKGROUND,
		D3DCOLOR_ARGB(255,255,255,255));

	for(int i = 0; i < 3; ++i)
	{
		GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.2f, 0.0f),
			D3DXVECTOR3(265, 235 + i * 160, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f,0.0f),
			spriteObj,
			GRAPHICS_SAVE_BACKGROUND,
			D3DCOLOR_ARGB(255,255,255,255));


	}

	for(int i = 0; i < 4; ++i)
	{
		GManager->DrawButton(D3DXVECTOR3(0.5f, 0.5f, 0.5f),
			D3DXVECTOR3(saveloadButtons[i].getPos().x, saveloadButtons[i].getPos().y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			saveloadButtons[i].getRect(), 
			spriteObj, 
			GRAPHICS_SAVE_BUTTONS, 
			saveloadButtons[i].width, 
			saveloadButtons[i].height,
			saveloadButtons[i].getColor());
	}
}

void IOManager::savegame(int fileNumber, Player *player, Overworld *overworld){
	//////////////////////////////////////////////////////////
	//  INFO:  Opens up save file
	savedGameFile.open("SavedGames.txt");
	for(int i = 0; i < 3; ++i){
		m_gameSave[fileNumber][0+i*4] = player->getCharacter(i)->getCurrentJob();
		m_gameSave[fileNumber][1+i*4] = player->getCharacter(i)->getJobLevel(WARRIOR);
		m_gameSave[fileNumber][2+i*4] = player->getCharacter(i)->getJobLevel(ARCHER);
		m_gameSave[fileNumber][3+i*4] = player->getCharacter(i)->getJobLevel(GREYMAGE);
	}
	for(int i = 0; i < MAXSAVES; ++i){
		for(int j = 0; j < 12; ++j){
			savedGameFile << m_gameSave[i][j];
			savedGameFile << '!';
		}
		savedGameFile << '\n';
	}
	savedGameFile.close();
};

void IOManager::loadgame(int fileNumber, Player *player, Overworld *overworld){
	std::string temp;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 4; ++j){
			player->setActiveJob(i, m_gameSave[fileNumber][0+i*4]);
			player->setCharacterLevel(i, 0, m_gameSave[fileNumber][1+i*4]);
			player->setCharacterLevel(i, 1, m_gameSave[fileNumber][2+i*4]);
			player->setCharacterLevel(i, 2, m_gameSave[fileNumber][3+i*4]);
		}
	}
};

void IOManager::loadSaves(){
	std::ifstream trial("SavedGames.txt");
	for(int i = 0; i < MAXSAVES; ++i){
		getline(trial, m_saveString[i]);
	}
	trial.close();
	std::string temp;// = m_saveString[0];
	temp.clear();
	int iter = 0;
	for(int i = 0; i < MAXSAVES; ++i){
		for(int j = 0; iter < 12; ++j){  //  TODO:  Replace 11 with all player data to save (Make a #define)
			char temporary = m_saveString[i][j];
			if(temporary == '!'){
				int testing = stoi(temp);
				m_gameSave[i][iter] = stoi(temp);
				++iter;
				temp.clear();
				continue;
			}
			temp += temporary;
		}
		iter = 0;
	};
	trial.close();
};