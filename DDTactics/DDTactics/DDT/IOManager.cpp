#include "IOManager.h"


IOManager::IOManager()
{}



IOManager::~IOManager()
{}


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

	for(int i = 0; i < 4; i++)
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
	//  INFO:  Saved game files converted to strings.  
	//			Initialized as empty strings

	for(int i = 0; i < 3; ++i){
		m_savedGame[i] = '\0';

	}

	iterator = 0;

}


void IOManager::update(InputManager *IManager, Cursor* cursor, int &game_state, float dt)
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
			game_state = OVERWORLD;
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