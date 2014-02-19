#include "Menu.h"

Menu::Menu()
{
	
}

Menu::~Menu()
{}


Menu* Menu::instance()
{
	static Menu menu;
	return &menu;
}

void Menu::Init( )
{
	menuState = MENU_MAIN;
	m_buttons.clear();

	//RDATA struct moved to MiscStructs.h
	RData uiData[] = 
	{
		{0, 0, 120, 500, 150, 280,false},
		{245, 0, 340, 500, 150, 350, false},
		{121, 0, 230, 500, 150, 420, false},
		{345, 0, 460, 300, 150, 500, false},
	};


	//Simplified functions into a for loop
	for(int i = 0; i < MAX_BUTTONS; i++)
	{
		Button temp;
		RECT rect;
		// NewGame
		rect.top = uiData[i].t; 
		rect.left = uiData[i].l;
		rect.bottom =  uiData[i].b; 
		rect.right =  uiData[i].r;
		temp.setPos( uiData[i].x, uiData[i].y);
		temp.setHighlight( uiData[i].highlight);
		temp.setRect(rect);
		m_buttons.push_back(temp);

	}


}

void Menu::Update(Cursor *cursor, SoundManager *SManager, InputManager *IManager, int &game_state, float dt)
{
	//Play opening fanfare
	SManager->playStream(SONG_MENU);
	
	//No longer need to update keyboard/mouse input, they're taken care of in DDTactics update
	//Cursor restraints are also taken care of in DDTactics update
	switch(menuState)
	{

	case MENU_MAIN:
		if( IManager->get_mouseX() || IManager->get_mouseY() )
		{
			for(auto &buttons: m_buttons)
			{
				//added isOn function to button to simplify testing if the cursor is on the button
				if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 3))
				{
					//added setColor function to simplify change in text color
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

		if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
		{
			if(!IManager->push_button(DIK_9))
			{
				IManager->set_button(DIK_9, true);
				int selected = 0;

				for(int i = 0; i < 4; i++)
				{
					if(m_buttons[i].isHighlighted())
						selected = i;
				}

				switch(selected)
				{
				case 0:
					SManager->stopStream();
					//game_state = OVERWORLD;
					game_state	=	INTRO;
					break;
				case 1:

					game_state = LOAD;
					break;
				case 2:
					game_state = OPTIONS;
					break;
				case 3:
					game_state = QUIT;
					break;
				}
			}
			else
				IManager->set_button(DIK_9, false);
		}
		break;

	/*case MENU_LOAD:
		game_state = LOAD;
		break;*/
	}

}

void Menu::Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt)
{
	switch(menuState)
	{
	case MENU_MAIN:
		GManager->Draw2DObject(D3DXVECTOR3(0.70f, 0.50f, 1.0f), 
								D3DXVECTOR3(310.0f, 200.0f, 0.0f),
								D3DXVECTOR3(0.0f,0.0f,0.0f), 
								spriteObj, 
								GRAPHICS_MENU, 
								D3DCOLOR_ARGB(255,255,255,255));
		
		for(auto &buttons: m_buttons)
		{
			GManager->DrawButton(D3DXVECTOR3(0.5f, 0.5f, 0.5f), 
								D3DXVECTOR3(buttons.getPos().x, buttons.getPos().y, 0.0f),
								D3DXVECTOR3(0.0f,0.0f,0.0f), 
								buttons.getRect(),
								spriteObj,
								GRAPHICS_MENU_BUTTONS, 
								buttons.width, 
								buttons.height,
								buttons.getColor());  
		}
		break;
	}
}

