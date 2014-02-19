#include "Intro.h"



IntroMenu::IntroMenu()
{}

IntroMenu::~IntroMenu()
{}

IntroMenu* IntroMenu::instance()
{
	static IntroMenu intro_menu;
	return &intro_menu;
}


void IntroMenu::init()
{
	//initialize buttons
	RData temp_button[] = 
	{
		{0, 0, 100, 130, 700, 40, false },
		
	};

	for(int i = 0; i < MAX_INTRO_BUTTONS; i++)
	{
		Button temp;
		RECT r;

		r.top = temp_button[i].t;
		r.left = temp_button[i].l;
		r.right = temp_button[i].r;
		r.bottom = temp_button[i].b;

		temp.setRect(r);
		temp.setPos(temp_button[i].x, temp_button[i].y);
		temp.setHighlight(temp_button[i].highlight);

		

		introMenuButtons.push_back(temp);	

		
	
	}

	for(int i = 0; i < MAX_INTRO_BUTTONS; i++)
	{
		introMenuButtons[i].setColor(D3DCOLOR_XRGB(255,255,255));
	}

	/*RData temp_border[] =
	{
		{},
	
	};*/


}

void IntroMenu::Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt)
{
	//  INFO:  If the player clicks the upper right portion of the screen (Skip option for intro) then change the gamestate to overworld.
	if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
		if((cursor->cursorPos.x > 650 && cursor->cursorPos.x < 750) && (cursor->cursorPos.y > 10 && cursor->cursorPos.y < 80)){
			SManager->stopStream();
			game_state = OVERWORLD;
		}
	}
}

void IntroMenu::Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt)
{
	GManager->Draw2DObject( D3DXVECTOR3(0.8f, 0.60f, 1.0f),
							D3DXVECTOR3(400.0f, 220.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							spriteObj,
							GRAPHICS_INTRO,
							D3DCOLOR_ARGB(255,255,255,255)
							);
	//  INFO:  Draws the "Skip" button
	for(unsigned int i = 0; i < introMenuButtons.size(); i++)
	{
		GManager->DrawButton( D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(introMenuButtons[i].getPos().x, introMenuButtons[i].getPos().y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			introMenuButtons[i].getRect(), spriteObj, GRAPHICS_BUTTON,
			introMenuButtons[i].width, introMenuButtons[i].height, introMenuButtons[i].getColor());
	
	}
								
}