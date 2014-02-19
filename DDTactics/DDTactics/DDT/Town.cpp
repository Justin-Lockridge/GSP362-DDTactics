#include "Town.h"

Town::Town()
{
	shop_state = SHOP_SELECTION;
	shop_sprite_pos.clear();
}

Town::~Town()
{}

Town* Town::instance()
{
	static Town town;
	return &town;
}

void Town::init()
{
	main = false;
	weapons = false;
	chest= false;
	helm = false;
	accesory = false;
	potions = false;
	buyMenu = false;
	// T, L, B, R, X, Y, Highlighted
	RData temp_menu_pos[] = 
	{
		{230, 20, 310, 270, 40, 225, false},
		{270, 20, 350, 270, 40, 265, false},
		{305, 20, 385, 270, 40, 300, false},
		{345, 20, 425, 270, 40, 340, false},
	};

	//Positons for the shop buttons
	for(int i = 0; i < MAX_SHOP_SPRITE_POSITIONS; i++)
	{
		Button temp;
		RECT rect;

		rect.top = temp_menu_pos[i].t;
		rect.left = temp_menu_pos[i].l;
		rect.bottom = temp_menu_pos[i].b;
		rect.right = temp_menu_pos[i].r;

		temp.setPos(temp_menu_pos[i].x, temp_menu_pos[i].y);
		temp.setHighlight(temp_menu_pos[i].highlight);
		temp.setRect(rect);

		shop_sprite_pos.push_back(temp);		
	}
	// T, L, B, R, X, Y, Highlighted
	RData temp_mini_menu_pos[] = 
	{
		{100,  50, 190, 120,  50, 200, false},
		{100, 100, 190, 170, 100, 200, false},
		{100, 150, 190, 220, 150, 200, false},
		{100, 200, 190, 270, 200, 200, false},
		{100, 250, 190, 320, 250, 200, false},
	};

	//Positons for the shop buttons
	for(int i = 0; i < MAX_SHOP_BUTTONS_POSITIONS; i++)
	{
		Button temp;
		RECT rect_mini;

		rect_mini.top = temp_mini_menu_pos[i].t;
		rect_mini.left = temp_mini_menu_pos[i].l;
		rect_mini.bottom = temp_mini_menu_pos[i].b;
		rect_mini.right = temp_mini_menu_pos[i].r;

		temp.setPos(temp_mini_menu_pos[i].x, temp_mini_menu_pos[i].y);
		temp.setHighlight(temp_mini_menu_pos[i].highlight);
		temp.setRect(rect_mini);

		shop_mini_buttons_pos.push_back(temp);		
	}
}

void Town::update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, int &game_state, float dt)
{
	SManager->playStream(SONG_TOWN);

	switch(shop_state)
	{
	case SHOP_SELECTION:
		main = true;
		buyMenu = false;
		break;
	case SHOP_WEAPONS:
		weapons = true;
		buyMenu = true;
		if(IManager->push_button(DIK_BACKSPACE))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			weapons = false;
			buyMenu = false;
		}
		break;
	case SHOP_HEAD:
		helm = true;
		if(IManager->push_button(DIK_BACKSPACE))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			helm =false;
			buyMenu = false;
		}
		break;
	case SHOP_CHEST:
		chest = true;
		if(IManager->push_button(DIK_BACKSPACE))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			chest = false;
			buyMenu = false;
		}
		break;
	case SHOP_ACCESORY:
		accesory = true;
		if(IManager->push_button(DIK_BACKSPACE))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			accesory = false;
			buyMenu = false;
		}
		break;
	case SHOP_POTIONS:
		potions = true;
		if(IManager->push_button(DIK_BACKSPACE))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			potions = false;
			buyMenu = false;
		}
		break;
	default:
		break;
	}

	if(!buyMenu)
	{
		//Check for cursor over buy/sell/fitting/exit menu
		for(int i = 0; i < MAX_SHOP_SPRITE_POSITIONS; i++)
		{
			if(shop_sprite_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				shop_sprite_pos[i].setHighlight(true);
				shop_sprite_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				shop_sprite_pos[i].setHighlight(false);
				shop_sprite_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			//Check for cursor over menu option and left mouse button click
			if(shop_sprite_pos[i].isHighlighted() && IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				//Switch states accordingly
				switch(i)
				{
				case 0: // buy
					main = false;
					shop_state = SHOP_WEAPONS;
					break;
				case 1: // sell
					//Needs to open up players inventory
					main = false;
					break;
				case 2: // Fitting
					//Not sure if we are using
					main = false;
					break;
				case 3: // Exit
					main = false;
					buyMenu = false;
					shop_state = SHOP_SELECTION;
					game_state = OVERWORLD;
					break;
				}
			}
		}
	}

	if(buyMenu)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_SHOP_BUTTONS_POSITIONS; i++)
		{
			if(shop_mini_buttons_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				shop_mini_buttons_pos[i].setHighlight(true);
				shop_mini_buttons_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				shop_mini_buttons_pos[i].setHighlight(false);
				shop_mini_buttons_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			//Check for cursor over menu option and left mouse button click
			if(shop_mini_buttons_pos[i].isHighlighted() && IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				//Switch states accordingly
				switch(i)
				{
				case 0: // Weapons
					chest = false;
					helm = false;
					accesory = false;
					potions = false;
					shop_state = SHOP_WEAPONS;
					break;
				case 1: // Head
					weapons = false;
					chest = false;
					accesory = false;
					potions = false;
					shop_state = SHOP_HEAD;
					break;
				case 2: // Chest
					weapons = false;
					helm = false;
					accesory = false;
					potions = false;
					shop_state = SHOP_CHEST;
					break;
				case 3: // Accesory
					weapons = false;
					chest = false;
					helm = false;
					potions = false;
					shop_state =SHOP_ACCESORY;
					break;
				case 4: // Potions
					weapons = false;
					chest = false;
					helm = false;
					accesory = false;
					shop_state = SHOP_POTIONS;
					break;
				}
			}
		}
	}
}

void Town::render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt)
{
	//Shop Selection
	if(main)
	{
		GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
			D3DXVECTOR3(240.0f, 250.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_KEEP, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(40.0f, 225.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_WEAPON_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(40.0f, 260.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_HELM_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(40.0f, 300.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_CHEST_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(40.0f, 340.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_ACCESORY_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));
	}
	//Buying Menus
	if(buyMenu)
	{
		if(weapons)
		{
			GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
				D3DXVECTOR3(240.0f, 250.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				spriteObj, GRAPHICS_SHOP_WEAPONS, 
				D3DCOLOR_ARGB(255,255,255,255));
		}
		if(helm)
		{
			GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
				D3DXVECTOR3(240.0f, 250.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				spriteObj, GRAPHICS_SHOP_HELMS, 
				D3DCOLOR_ARGB(255,255,255,255));
		}
		if(chest)
		{
			GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
				D3DXVECTOR3(240.0f, 250.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				spriteObj, GRAPHICS_SHOP_CHESTS, 
				D3DCOLOR_ARGB(255,255,255,255));
		}
		if(accesory)
		{
			GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
				D3DXVECTOR3(240.0f, 250.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				spriteObj, GRAPHICS_SHOP_ACCESORY, 
				D3DCOLOR_ARGB(255,255,255,255));
		}
		if(potions)
		{
			GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
				D3DXVECTOR3(240.0f, 250.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				spriteObj, GRAPHICS_SHOP_POTIONS, 
				D3DCOLOR_ARGB(255,255,255,255));
		}

		//Draw the five buttons
		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(50.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_WEAPON_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(100.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_HELM_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(150.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_CHEST_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(200.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_ACCESORY_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));

		GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.55f, 0.4f),
			D3DXVECTOR3(250.0f, 180.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_POTION_BUTTON, 
			D3DCOLOR_ARGB(255,255,255,255));
	}
}