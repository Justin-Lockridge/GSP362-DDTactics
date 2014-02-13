#include "StatusMenu.h"



StatusMenu::StatusMenu()
{}

StatusMenu::~StatusMenu()
{}

StatusMenu* StatusMenu::instance()
{
	static StatusMenu status_menu;
	return &status_menu;
}


void StatusMenu::init()
{
	//initialize buttons
	RData temp_button[] = 
	{
		{0, 0, 100, 100, 700, 0, false },
		{100, 0, 200, 100, 700, 100, false},
		{200, 0, 300, 100, 700, 200, false},
		{300, 0, 400, 100, 700, 300, false},
		{400,0, 500, 100, 700, 400, false},
		{500, 0, 600, 100, 700, 500, false},
		{600, 0, 700, 100, 700, 600, false},
		
	};

	for(int i = 0; i < MAX_STATUS_BUTTONS; i++)
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

		

		status_menu_buttons.push_back(temp);	

		
	
	}

	for(int i = 0; i < MAX_STATUS_BUTTONS; i++)
	{
		status_menu_buttons[i].setColor(D3DCOLOR_XRGB(255,255,255));
	}

	/*RData temp_border[] =
	{
		{},
	
	};*/


}

void StatusMenu::Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player &player, int &game_state, float dt)
{
	if(IManager->push_button(DIK_BACKSPACE))
		game_state = OVERWORLD;
}

void StatusMenu::Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt)
{
	GManager->Draw2DObject( D3DXVECTOR3(0.8f, 0.75f, 1.0f),
							D3DXVECTOR3(300.0f, 100.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							spriteObj,
							GRAPHICS_STATUS_BACKGROUND,
							D3DCOLOR_ARGB(255,255,255,255)
							);

	for(int i = 0; i < status_menu_buttons.size(); i++)
	{
		GManager->DrawButton( D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(status_menu_buttons[i].getPos().x, status_menu_buttons[i].getPos().y, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			status_menu_buttons[i].getRect(), spriteObj, GRAPHICS_STATUS_BUTTONS,
			status_menu_buttons[i].width, status_menu_buttons[i].height, status_menu_buttons[i].getColor());
	
	}
								
}