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
	status_state = STATUS_MAIN;

	//initialize buttons
	RData temp_button[] = 
	{
		{0, 0, 45, 100, 750, 50, false },
		{65, 0, 120, 100, 750, 80, false},
		{140, 0, 195, 100, 750, 140, false},
		{215, 0, 270, 100, 750, 200, false},
		{290,0, 345, 100, 750, 260, false},
		{365, 0, 430, 100, 750, 330, false},
		{450, 0, 505, 100, 750, 420, false},
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


	RData tempItem_Buttons[] =
	{
		{95, 0, 140, 120, 125, 70, false },
		{220, 0,265, 120, 385, 70, false },
		{345, 0, 390, 120, 625, 70, false},
	
	};
	
	for(int i = 0; i < 3; i++)
	{
		Button temp;
		RECT r;

		r.top = tempItem_Buttons[i].t;
		r.left = tempItem_Buttons[i].l;
		r.right = tempItem_Buttons[i].r;
		r.bottom = tempItem_Buttons[i].b;

		temp.setRect(r);
		temp.setPos(tempItem_Buttons[i].x, tempItem_Buttons[i].y);
		temp.setHighlight(tempItem_Buttons[i].highlight);

		status_item_buttons.push_back(temp);	

	}
}

void StatusMenu::Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt)
{
	switch(status_state)
	{
	case STATUS_MAIN:
		for(int i = 0; i < MAX_STATUS_BUTTONS; i++)
		{
			status_menu_buttons[i].checkOn(cursor->cursorPos.x, cursor->cursorPos.y, 3);

			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(status_menu_buttons[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9, true);

						switch(i+1)
						{
						case STATUS_ITEM:
							status_state = STATUS_ITEM;
							break;
						case STATUS_ABILITY:
							status_state = STATUS_ABILITY;
							break;
						case STATUS_MAGIC:
							status_state = STATUS_MAGIC;
							break;
						case STATUS_JOB:
							status_state = STATUS_JOB;
							break;
						case STATUS_STATUS:
							status_state = STATUS_STATUS;
							break;
						case STATUS_ORDER:
							status_state = STATUS_ORDER;
							break;
						case STATUS_BACK:
							status_state = STATUS_BACK;
							break;
						
						}
					}
			
				}
			
			}else
				IManager->set_button(DIK_9, false);
		}
		//TODO:  Draw characters, be able to click on character to activate


		//TODO:  Draw character status on the bottom

		//TODO:  Draw amount of money on the bottom right, possible draw time
		break;
	case STATUS_ITEM:
		for(int i = 0; i < status_item_buttons.size(); i++)
		{
			
			status_item_buttons[i].checkOn(cursor->cursorPos.x, cursor->cursorPos.y, 2);
		
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(status_item_buttons[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9, true);
						switch(i)
						{
						case STATUSMENUITEM_USE:
							status_state = STATUS_ITEM_USE;
							break;
						case STATUSMENUITEM_ORGANIZE:
							status_state = STATUS_ITEM_ORGANIZE;
							break;
						case STATUSMENUITEM_BACK:
							status_state = STATUS_MAIN;
							break;
						
						
						}
					}
				}
			
			
			}else
				IManager->set_button(DIK_9, false);
		
		
		
		}

		//TODO:  Draw Text for each item in inventory and how many there are

		
		
		
		break;
	case STATUS_ABILITY:
		break;
	case STATUS_MAGIC:
		break;
	case STATUS_JOB:
		break;
	case STATUS_STATUS:
		break;
	case STATUS_ORDER:
		break;
	case STATUS_BACK:
		game_state = OVERWORLD;
		status_state = STATUS_MAIN;
		break;
	case STATUS_ITEM_USE:
		//NOTE:  Use currently has no use aside from being able to highlight the items and display item description

		//TODO:  Draw Text for each item in inventory and how many there are

		//TODO:  If highlighted, the item will display its description at the bottom

		//TODO:  Draw Item Description at the bottom
		break;
	case STATUS_ITEM_ORGANIZE:
		//NOTE: Organize currently has no use, return state to STATUS_ITEM
		game_state = STATUS_ITEM;
		break;
	}
}

void StatusMenu::Render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, Player * player, float dt)
{
	switch(status_state)
	{

	case STATUS_ITEM:
		GManager->Draw2DObject(D3DXVECTOR3(0.775f, 0.575f, 1.0f),
							   D3DXVECTOR3(312.0f, 180.f, 0.0f),
							   D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							   spriteObj,
							   GRAPHICS_ITEM_BACKGROUND,
							   D3DCOLOR_ARGB(255,255,255,255)
							   );

		for(int i = 0; i < status_item_buttons.size(); i++)
		{
			GManager->DrawButton( D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(status_item_buttons[i].getPos().x, status_item_buttons[i].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				status_item_buttons[i].getRect(), spriteObj, GRAPHICS_ITEM_BUTTONS,
				status_item_buttons[i].width, status_item_buttons[i].height, status_item_buttons[i].getColor());

		
		}


		break;
	case STATUS_ABILITY:
		break;
	case STATUS_MAGIC:
		break;
	case STATUS_JOB:
		break;
	case STATUS_STATUS:
		break;
	case STATUS_ITEM_USE:
		//NOTE:  Use currently has no use aside from being able to highlight the items and display item description

		//TODO:  Draw Text for each item in inventory and how many there are

		//TODO:  If highlighted, the item will display its description at the bottom

		//TODO:  Draw Item Description at the bottom

		//TODO:  RIGHT MOUSE CLICK to return back to STATUS_ITEM
		break;
	default:
		GManager->Draw2DObject( D3DXVECTOR3(0.775f, 0.575f, 1.0f),
			D3DXVECTOR3(312.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj,
			GRAPHICS_STATUS_BACKGROUND,
			D3DCOLOR_ARGB(255,255,255,255)
			);

		for(unsigned int i = 0; i < status_menu_buttons.size(); i++)
		{
			GManager->DrawButton( D3DXVECTOR3(2.0f, 2.0f, 1.0f),
				D3DXVECTOR3(status_menu_buttons[i].getPos().x, status_menu_buttons[i].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				status_menu_buttons[i].getRect(), spriteObj, GRAPHICS_STATUS_BUTTONS,
				status_menu_buttons[i].width, status_menu_buttons[i].height, status_menu_buttons[i].getColor());

		}
		break;

	}

}