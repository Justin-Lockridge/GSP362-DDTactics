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
	};

	for(int i = 0; i < sizeof(uiData)/sizeof(uiData[0]); ++i)
	{
		DoTheThing(uiData[i],  m_buttons);
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
void CDirectXFramework::UpdateMenu()
{
	if(!m_musicChannel){																	//check if channel im streaming to is in use
		fmodSystem->playSound(FMOD_CHANNEL_FREE, m_menuMusic, false, &m_musicChannel);		//stream and save the channel im using
		//m_musicChannel->setVolume(0.3f);
		
	}
	
	//////////////////////////////////////////////////////
	//Smyth -											//
	//Moved buffer and mouseState to DirectxFramework.h	//
	//Moved keyboard and mouse acquire/getdevice state  //
	//to DirectxFramework.cpp update function.			//
	//////////////////////////////////////////////////////
	

	m_cursor.x+=mouseState.lX;
	m_cursor.y+=mouseState.lY;

	
	if(m_cursor.x <0)		m_cursor.x=0;
	if(m_cursor.y <0)		m_cursor.y=0;
	if(m_cursor.x >800)		m_cursor.x=800;
	if(m_cursor.y >600)		m_cursor.y=600;

	if(mouseState.lX || mouseState.lY){
		//for each button
		for(auto &buttons: m_buttons){
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
	if(mouseState.rgbButtons[0] & 0x80){
		if(!m_keyPressed[DIK_9]){
			m_keyPressed[DIK_9] = true;
			int selected=94;
			for(int i = 0;i<4;i++){
				if(m_buttons[i].isHighlighted())
					selected = i;
			}
			switch(selected){
			case 0:
				//m_gameState = NEWGAME;
				//GameInit();
				break;
			case 1:
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



	/*if(buffer[DIK_B] & 0x80){
		if(!m_keyPressed[DIK_B]){
			m_keyPressed[DIK_B] = true;
		m_gameState = BATTLE;
		}
	}else m_keyPressed[DIK_B]=false;*/
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
		/*	m_pD3DDevice->EndScene();
		}
		m_pD3DDevice->Present(0, 0, 0, 0);
	}*/
}