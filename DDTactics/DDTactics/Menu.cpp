#include "DirectXFramework.h"

struct RData{
	int t, l, b, r, x, y;
	bool  highlight;
};

void DoTheThing(RData &rd, std::vector<Button> & buttons )
{
	Button temp;
	RECT rect;

	// NewGame
	rect.top = rd.t; rect.left = rd.l;
	rect.bottom = rd.b; rect.right = rd.r;
	temp.setPos(rd.x,rd.y);
	temp.setHighlight(rd.highlight);
	temp.setRect(rect);
	buttons.push_back(temp);
}


void CDirectXFramework::InitMenu()
{
	m_musicChannel->stop();
	m_musicChannel = 0;
	m_gameState = MENU;
	m_buttons.clear();

	RData uiData[]  = 
	{
		{0, 0, 120, 500, 150, 280,false},
		{245, 0, 340, 500, 150, 350, false},
		{121, 0, 230, 500, 150, 420, false},
		{345, 0, 460, 300, 150, 500, false},
		{60,  0, 140, 280, 110, 235, false},
		{140, 0, 210, 280, 110, 395, false},
		{215, 0, 285, 280, 110, 555, false},
		{285, 0, 360, 450, 150, 80,  false},
	};

	for(int i = 0; i < sizeof(uiData)/sizeof(uiData[0]); ++i)
	{
		DoTheThing(uiData[i],  m_buttons);
	}
	///////////////////////////////////////////////////////////
	//  INFO:  Saved game files converted to strings.  
	//			Initialized as empty strings
	for(int i = 0; i < 3; ++i){
		m_savedGame[i] = '\0';
	}
	//Button temp;
	//RECT rect;

	//// NewGame
	//rect.top = 0; rect.left = 0;
	//rect.bottom = 120; rect.right = 500;
	//temp.setPos(150,280);
	//temp.setHighlight(false);
	//temp.setRect(rect);
	//m_buttons.push_back(temp);

	//// Continue
	//rect.top = 245; rect.left = 0;
	//rect.bottom = 340; rect.right = 500;
	//temp.setPos(150,350);
	//temp.setHighlight(false);
	//temp.setRect(rect);
	//m_buttons.push_back(temp);

	//// Options
	//rect.top = 121; rect.left = 0;
	//rect.bottom = 230; rect.right = 500;
	//temp.setPos(150,420);
	//temp.setHighlight(false);
	//temp.setRect(rect);
	//m_buttons.push_back(temp);

	//// Quit
	//rect.top = 345; rect.left = 0;
	//rect.bottom = 460; rect.right = 300;
	//temp.setPos(150,500);
	//temp.setHighlight(false);
	//temp.setRect(rect);
	//m_buttons.push_back(temp);
}
int iterator = 0;
void CDirectXFramework::UpdateMenu()
{
	if(!m_musicChannel)
	{																	//check if channel im streaming to is in use
		fmodSystem->playSound(FMOD_CHANNEL_FREE, m_menuMusic, false, &m_musicChannel);		//stream and save the channel im using
		//m_musicChannel->setVolume(0.3f);
		
	}
	ZeroMemory(buffer, sizeof(buffer));
	// Get the input device state
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState( sizeof(buffer), &buffer );

	if(FAILED(hr))
	{
		hr = m_pDIKeyboard->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIKeyboard->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;

		// Read the device state again
		m_pDIKeyboard->GetDeviceState(sizeof(buffer), buffer);
	}
	//////////////////////////////////////////////////////////////////////////
	// Get and Acquire Mouse Input
	//////////////////////////////////////////////////////////////////////////
	// Stores our mouse state for an 8 button mouse.
	ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the input device state
	hr = m_pDIMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &mouseState );
	if(FAILED(hr))
	{
		hr = m_pDIMouse->Acquire();

		// Device has probably been lost if failed, if so keep trying to get it until it’s found.
		while( hr == DIERR_INPUTLOST)
		{
			hr = m_pDIMouse->Acquire();
		}

		// If we failed for some other reason
		if(FAILED(hr))
			return;
		// Read the device state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}
	

	m_cursor.x+=mouseState.lX;
	m_cursor.y+=mouseState.lY;

	
	if(m_cursor.x <0)		m_cursor.x=0;
	if(m_cursor.y <0)		m_cursor.y=0;
	if(m_cursor.x >800)		m_cursor.x=800;
	if(m_cursor.y >600)		m_cursor.y=600;

	/////////////////////////////////////////////////////////////
	//  INFO:  Menu substates
	/////////////////////////////////////////////////////////////
	switch(m_menuState)
	{
	case MENU_MAIN:
		if(mouseState.lX || mouseState.lY)
		{
			//for each button
			for(auto &buttons: m_buttons)
			{
				RECT buttonRect = buttons.getRect();
				int width = (buttonRect.right - buttonRect.left)/3;
				int height = (buttonRect.bottom - buttonRect.top)/3;
				if(m_cursor.x<buttons.getPos().x+width && m_cursor.x>buttons.getPos().x-width &&
					m_cursor.y<buttons.getPos().y+height && m_cursor.y>buttons.getPos().y-height)
					buttons.setHighlight(true);
				else
					buttons.setHighlight(false);
			}
		}
		if(mouseState.rgbButtons[0] & 0x80)
		{
			if(!m_keyPressed[DIK_9])
			{
				m_keyPressed[DIK_9] = true;
				int selected=94;
				for(int i = 0;i<4;i++)
				{
					if(m_buttons[i].isHighlighted())
						selected = i;
				}
				switch(selected)
				{
				case 0:
					///////////////////////////////////////////////////////////////
					//  INFO:  Player chooses new game.  Menu music stops, sets
					//			state to overworld, and plays overworld music.
					///////////////////////////////////////////////////////////////
					m_musicChannel->stop();
					overworld.init(m_pD3DDevice, fmodSystem);
					m_gameState = OVERWORLD;																
					fmodSystem->playSound(FMOD_CHANNEL_FREE, m_overWorldMusic, false, &m_musicChannel);	
					//GameInit();
					break;
				case 1:
					loadGame();
					m_menuState = MENU_LOAD;
					//m_gameState = CONTINUE;
					//m_check = 3;
					break;
				case 2:
					//m_gameState = OPTIONS; // put credits within options
					break;
				case 3:
					m_gameState = QUIT;
					break;
				}
			} 
		}else m_keyPressed[DIK_9] = false;
		break;
	case MENU_LOAD:
		{
		//////////////////////////////////////////////////////////////
		//  INFO:  For testing saving and loading
		if(buffer[DIK_S] & 0x80)
		{
			int temporary = 0;
			if(!m_keyPressed[DIK_S])
			{
				m_keyPressed[DIK_S] = true;
				++iterator;
				m_player.setJobLevel(GREYMAGE, iterator);
				saveGame(0);
			}

		}
		else
			m_keyPressed[DIK_S] = false;

		if(buffer[DIK_L] & 0x80)
		{
			if(!m_keyPressed[DIK_L])
			{
				m_keyPressed[DIK_L] = true;
				loadGame();
			}

		}
		else
			m_keyPressed[DIK_L] = false;
		///////////////////////////////////////////////////////////////////////////////////////////////
		//  INFO:  Loads save files from a text file in order to print character info to screen
		if(mouseState.lX || mouseState.lY)
		{
			//for each button
			for(auto &buttons: m_buttons)
			{
				RECT buttonRect = buttons.getRect();
				int width = (buttonRect.right - buttonRect.left)/3;
				int height = (buttonRect.bottom - buttonRect.top)/3;
				if(m_cursor.x<buttons.getPos().x+width && m_cursor.x>buttons.getPos().x-width &&
					m_cursor.y<buttons.getPos().y+height && m_cursor.y>buttons.getPos().y-height)
					buttons.setHighlight(true);
				else
					buttons.setHighlight(false);
			}
		}
		if(mouseState.rgbButtons[0] & 0x80)
		{
			if(!m_keyPressed[DIK_9])
			{
				m_keyPressed[DIK_9] = true;
				int selected=94;
				for(int i = 4;i<8;i++)
				{
					if(m_buttons[i].isHighlighted())
						selected = i;
				}
				switch(selected)
				{
				case 4:
					saveGame(0);
					break;
				case 5:
					saveGame(1);
					//m_gameState = CONTINUE;
					//m_menuState = MENU_LOAD;
					//m_check = 3;
					break;
				case 6:
					saveGame(2);
					//m_gameState = OPTIONS; // put credits within options
					break;
				case 7:
					m_menuState = MENU_MAIN;
					//m_gameState = QUIT;
					break;
				}
			} 
		}else m_keyPressed[DIK_9] = false;
		break;
				   }

	}

	/*if(buffer[DIK_B] & 0x80){
	if(!m_keyPressed[DIK_B]){
	m_keyPressed[DIK_B] = true;
	m_gameState = BATTLE;
	}
	}else m_keyPressed[DIK_B]=false;*/

	//////////////For shop testing///////////////////////
	if(buffer[DIK_T] & 0x80)
	{
		if(!m_keyPressed[DIK_T])
		{
			m_gameState = TAVERN;
			m_keyPressed[DIK_T] = true;
		}

	}
	else
		m_keyPressed[DIK_T] = false;

}
void CDirectXFramework::RenderMenu()
{

	//////////////////////////////////////////////////////////////////////////////
	//Smyth - moved to DirectxFramework.cpp										//
	//		  unless all states are being done in Menu.cpp, the other files 	//
	//		  will have to re-declare these statements.						    //
	//////////////////////////////////////////////////////////////////////////////

	/*if(SUCCEEDED(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0))){
	if(SUCCEEDED(m_pD3DDevice->BeginScene())){*/
	switch(m_menuState){
	case MENU_MAIN:{
		if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK))){

		D3DXMATRIX rotMat, worldMat, transMat, scaleMat, WIT;

		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);

		D3DXMatrixScaling(&scaleMat, 0.70f, 0.50f, 1.0f);
		D3DXMatrixTranslation(&transMat, 310, 200, 0.0f);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		m_pD3DSprite->SetTransform(&worldMat);

		m_pD3DSprite->Draw(m_backGround, 0, &D3DXVECTOR3(m_backGroundInfo.Width * 0.5f, m_backGroundInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));


		// Draw the texture with the sprite object
		for(auto &buttons: m_buttons){
			RECT buttonRect = buttons.getRect();

			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat,0.5f, 0.5f, 0.5f);
			D3DXMatrixTranslation(&transMat, buttons.getPos().x, buttons.getPos().y, 0.0f);
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			m_pD3DSprite->SetTransform(&worldMat);
			// Draw the texture with the sprite object
			if(buttons.isHighlighted()){
				m_pD3DSprite->Draw(m_menuTextures, &buttonRect, &D3DXVECTOR3((buttonRect.right-buttonRect.left) * 0.5f, (buttonRect.bottom-buttonRect.top) * 0.5f, 0.0f),
					0, D3DCOLOR_ARGB(255, 255, 255, 0));
			}
			else{
				m_pD3DSprite->Draw(m_menuTextures, &buttonRect, &D3DXVECTOR3((buttonRect.right-buttonRect.left) * 0.5f, (buttonRect.bottom-buttonRect.top) * 0.5f, 0.0f),
					0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//////////////////////////////////////////////////////////////////
			//  INFO:  Breaks out of loop after drawing main menu buttons
			//////////////////////////////////////////////////////////////////
			if(buttons.getPos().x == 150 && buttons.getPos().y == 500)
				break;
		}
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);

		D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.0f);
		D3DXMatrixTranslation(&transMat, m_cursor.x, m_cursor.y, 0.0f);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		m_pD3DSprite->SetTransform(&worldMat);
		m_pD3DSprite->Draw(m_cursorTexture, 0, &D3DXVECTOR3(m_cursorInfo.Width * 0.5f, m_cursorInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pD3DSprite->End();
				   }
				   //m_pD3DDevice->EndScene();

				   //m_pD3DDevice->Present(0, 0, 0, 0);

				   break;
				   }
	case MENU_LOAD:{
		if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK))){
		//if(SUCCEEDED(m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0))){
		//if(SUCCEEDED(m_pD3DDevice->BeginScene())){
		//if(SUCCEEDED(m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK))){

		D3DXMATRIX rotMat, worldMat, transMat, scaleMat, WIT;

		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);

		D3DXMatrixScaling(&scaleMat,0.4f, 0.6f, 0.0f);			// Scaling
		D3DXMatrixTranslation(&transMat, 265, 235, 0.0f);	
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		m_pD3DSprite->SetTransform(&worldMat);

		m_pD3DSprite->Draw(m_saveBackground, 0, &D3DXVECTOR3(m_saveBackgroundInfo.Width * 0.5f, m_saveBackgroundInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));

		//////////////////////////////////////////////////////////////////////////
		//  INFO:  Draws the background image for 3 save files
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//  INFO:  Draws backgrounds for file saving
		for(int i = 0; i < 3; ++i){
			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat,0.4f, 0.2f, 0.0f);			// Scaling
			D3DXMatrixTranslation(&transMat, 265, 235 + i * 160, 0.0f);	
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			m_pD3DSprite->SetTransform(&worldMat);

			m_pD3DSprite->Draw(m_saveBackground, 0, &D3DXVECTOR3(m_saveBackgroundInfo.Width * 0.5f, m_saveBackgroundInfo.Height * 0.5f, 0.0f),
				0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		///////////////////////////////////////////////////////////////////////////////////
		//  INFO:  Draws the buttons for the save game sub menu
		for(int i = 4; i < 8; ++i){
			RECT buttonRect = m_buttons[i].getRect();

			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat,0.5f, 0.5f, 0.5f);
			D3DXMatrixTranslation(&transMat,  m_buttons[i].getPos().x,  m_buttons[i].getPos().y, 0.0f);
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			m_pD3DSprite->SetTransform(&worldMat);
			// Draw the texture with the sprite object
			if( m_buttons[i].isHighlighted()){
				m_pD3DSprite->Draw(m_saveMenuTextures, &buttonRect, &D3DXVECTOR3((buttonRect.right-buttonRect.left) * 0.5f, (buttonRect.bottom-buttonRect.top) * 0.5f, 0.0f),
					0, D3DCOLOR_ARGB(255, 255, 255, 0));
			}
			else{
				m_pD3DSprite->Draw(m_saveMenuTextures, &buttonRect, &D3DXVECTOR3((buttonRect.right-buttonRect.left) * 0.5f, (buttonRect.bottom-buttonRect.top) * 0.5f, 0.0f),
					0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
		/////////////////////////////////////////////////////
		//  INFO:  Draws the mouse cursor
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);

		D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.0f);
		D3DXMatrixTranslation(&transMat, m_cursor.x, m_cursor.y, 0.0f);
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

		m_pD3DSprite->SetTransform(&worldMat);
		m_pD3DSprite->Draw(m_cursorTexture, 0, &D3DXVECTOR3(m_cursorInfo.Width * 0.5f, m_cursorInfo.Height * 0.5f, 0.0f),
			0, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pD3DSprite->End();
				   }
				   //RECT rect;
				   //GetWindowRect(m_hWnd, &rect);
				   //rect.right = rect.right - rect.left;
				   //rect.bottom = rect.bottom - rect.top;
				   //rect.top = 50;			
				   //rect.left = 50;
				   //wchar_t buffer[512];
				   //ZeroMemory(buffer, sizeof(buffer));
				   //swprintf_s(buffer, 512, L"Load Game:  Choose a save file to load\n");
				   //m_pD3DFont->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));

				   ////////////////////////////////////////////////////////////////////////////////////////////
				   //  INFO:  Draws information about each save before the player chooses a file to load
				   ////////////////////////////////////////////////////////////////////////////////////////////
				   loadGame();
				   for(int i = 0; i < MAXSAVES; ++i){
					   RECT rect;
					   rect.top = 215 + 160 * i;	
					   rect.bottom = 240 + 160 * i;
					   rect.left = 200;
					   rect.right = 250;
					   wchar_t buffer[64];
					   ZeroMemory(buffer, sizeof(buffer));
					   //char temp = m_savedGame[i].at(0);
					   //int test = (int)m_savedGame[i][0];
					   //test = atoi(&m_savedGame[i][0]);
					   //char trial = m_savedGame[i].at(2);
					   //int test = atoi(m_savedGame[i][0]);
					   int curLevel = 99;
					   switch(atoi(&m_savedGame[i][0])){
					   case WARRIOR:
						   //curLevel = atoi(&m_savedGame[i][2]);
						   swprintf_s(buffer, 64, L"Warrior:  %d\nRogue:  %d\nGreyMage:  %d", m_gameSave[i][WARRIORLEVEL], m_gameSave[i][ROGUELEVEL], m_gameSave[i][GREYMAGELEVEL]);
						   break;
					   case ROGUE:
						   //curLevel = atoi(&m_savedGame[i][2]);
						   swprintf_s(buffer, 64, L"Warrior:  %d\nRogue:  %d\nGreyMage:  %d", m_gameSave[i][WARRIORLEVEL], m_gameSave[i][ROGUELEVEL], m_gameSave[i][GREYMAGELEVEL]);
						   break;
					   case GREYMAGE:
						   //curLevel = atoi(&m_savedGame[i][2]);
						   swprintf_s(buffer, 64, L"Warrior:  %d\nRogue:  %d\nGreyMage:  %d", m_gameSave[i][WARRIORLEVEL], m_gameSave[i][ROGUELEVEL], m_gameSave[i][GREYMAGELEVEL]);
						   break;
					   }
					   m_pD3DFont->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125, 0));
				   };


				   //m_pD3DDevice->EndScene();

				   //m_pD3DDevice->Present(0, 0, 0, 0);

				   break;
	}
	}

}

void CDirectXFramework::saveGame(int saveLocation){
	std::ofstream file("SaveFiles.txt");
	m_gameSave[saveLocation][CURRENTJOB]			=	m_player.getCurrentJob();
	m_gameSave[saveLocation][WARRIORLEVEL]			=	m_player.getJobLevel(WARRIOR);
	m_gameSave[saveLocation][ROGUELEVEL]			=	m_player.getJobLevel(ROGUE);
	m_gameSave[saveLocation][GREYMAGELEVEL]			=	m_player.getJobLevel(GREYMAGE);
	m_gameSave[saveLocation][CURRENTEXPERIENCE]		=	m_player.getCurrentExperience();
	m_gameSave[saveLocation][CURRENTGOLD]			=	m_player.getCurrentGold();
	for(int i = 0; i < MAXSAVES; ++i){
		for(int j = 0;  j < MAXPLAYERDATA; ++j){
			file << m_gameSave[i][j];
			file << ' ';
		};
		file << '\n';
	};
	file.close();
};

void CDirectXFramework::loadGame(){
	std::ifstream file("SaveFiles.txt");
	int iterator = 0;
	for(int i = 0; i < MAXSAVES; ++i){
		iterator = 0;
		std::getline(file, m_savedGame[i]);
		for(int j = 0; iterator < MAXPLAYERDATA; ++j){
			if(m_savedGame[i][j] == ' ')
				continue;
			int temp = atoi(&m_savedGame[i].at(j));
			m_gameSave[i][iterator] = atoi(&m_savedGame[i].at(j));
			++iterator;
		};
	};
	file.close();
};