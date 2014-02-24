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
	noMoney = false;
	// T, L, B, R, X, Y, Highlighted
	RData temp_menu_pos[] = 
	{
		{230, 20, 310, 270, 40, 225, false},
		{270, 20, 350, 270, 40, 265, false},
		{305, 20, 385, 270, 40, 300, false},
		//{345, 20, 425, 270, 40, 340, false},
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

	RData temp_weapon_pos[] = 
	{
		{240, 30, 280, 400, 215, 260, false},
		{285, 30, 325, 400, 215, 305, false},
		{330, 30, 370, 400, 215, 350, false},
		{375, 30, 415, 400, 215, 395, false},
	};
	//Positons for weapons to buy
	for(int i = 0; i < MAX_WEAPONS_BUTTONS_POTITIONS; i++)
	{
		Button temp;
		RECT rect_weapons;

		rect_weapons.top = temp_weapon_pos[i].t;
		rect_weapons.left = temp_weapon_pos[i].l;
		rect_weapons.bottom = temp_weapon_pos[i].b;
		rect_weapons.right = temp_weapon_pos[i].r;

		temp.setPos(temp_weapon_pos[i].x, temp_weapon_pos[i].y);
		temp.setHighlight(temp_weapon_pos[i].highlight);
		temp.setRect(rect_weapons);

		weapons_buttons_pos.push_back(temp);		
	}

	RData temp_helm_pos[] = 
	{
		{240, 30, 280, 400, 215, 260, false},
		//{285, 30, 325, 400, 215, 305, false},
		//{330, 30, 370, 400, 215, 350, false},
		//{375, 30, 415, 400, 215, 395, false},
	};
	//Positons for weapons to buy
	for(int i = 0; i < MAX_HELM__BUTTONS_POSITIONS; i++)
	{
		Button temp;
		RECT rect_helm;

		rect_helm.top = temp_helm_pos[i].t;
		rect_helm.left = temp_helm_pos[i].l;
		rect_helm.bottom = temp_helm_pos[i].b;
		rect_helm.right = temp_helm_pos[i].r;

		temp.setPos(temp_helm_pos[i].x, temp_helm_pos[i].y);
		temp.setHighlight(temp_helm_pos[i].highlight);
		temp.setRect(rect_helm);

		helm_button_pos.push_back(temp);		
	}

	RData temp_chest_pos[] = 
	{
		{240, 30, 280, 400, 215, 260, false},
		//{285, 30, 325, 400, 215, 305, false},
		//{330, 30, 370, 400, 215, 350, false},
		//{375, 30, 415, 400, 215, 395, false},
	};
	//Positons for weapons to buy
	for(int i = 0; i < MAX_CHEST_BUTTONS_POSITIONS; i++)
	{
		Button temp;
		RECT rect_chest;

		rect_chest.top = temp_chest_pos[i].t;
		rect_chest.left = temp_chest_pos[i].l;
		rect_chest.bottom = temp_chest_pos[i].b;
		rect_chest.right = temp_chest_pos[i].r;

		temp.setPos(temp_chest_pos[i].x, temp_chest_pos[i].y);
		temp.setHighlight(temp_chest_pos[i].highlight);
		temp.setRect(rect_chest);

		chest_button_pos.push_back(temp);	
	}
	RData temp_accesory_pos[] = 
	{
		{240, 30, 280, 400, 215, 260, false},
		//{285, 30, 325, 400, 215, 305, false},
		//{330, 30, 370, 400, 215, 350, false},
		//{375, 30, 415, 400, 215, 395, false},
	};
	//Positons for accesories to buy
	for(int i = 0; i < MAX_ACCESORY_BUTTONS_POSITIONS; i++)
	{
		Button temp;
		RECT rect_accesory;

		rect_accesory.top = temp_accesory_pos[i].t;
		rect_accesory.left = temp_accesory_pos[i].l;
		rect_accesory.bottom = temp_accesory_pos[i].b;
		rect_accesory.right = temp_accesory_pos[i].r;

		temp.setPos(temp_accesory_pos[i].x, temp_accesory_pos[i].y);
		temp.setHighlight(temp_accesory_pos[i].highlight);
		temp.setRect(rect_accesory);

		accesory_button_pos.push_back(temp);	
	}

	RData temp_potion_pos[] = 
	{
		{240, 30, 280, 400, 215, 260, false},
		{285, 30, 325, 400, 215, 305, false},
		{330, 30, 370, 400, 215, 350, false},
		{375, 30, 415, 400, 215, 395, false},
		{418, 30, 458, 400, 215, 438, false},
	};
	//Positons for potions to buy
	for(int i = 0; i < MAX_POTIONS_BUTTONS_POSITION; i++)
	{
		Button temp;
		RECT rect_potion;

		rect_potion.top = temp_potion_pos[i].t;
		rect_potion.left = temp_potion_pos[i].l;
		rect_potion.bottom = temp_potion_pos[i].b;
		rect_potion.right = temp_potion_pos[i].r;

		temp.setPos(temp_potion_pos[i].x, temp_potion_pos[i].y);
		temp.setHighlight(temp_potion_pos[i].highlight);
		temp.setRect(rect_potion);

		potion_button_pos.push_back(temp);	
	}
}

void Town::update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, int &game_state, float dt, Player *player)
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
		if(IManager->check_mouse_button(1))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			weapons = false;
			buyMenu = false;
		}
		break;
	case SHOP_HEAD:
		helm = true;
		buyMenu = true;
		if(IManager->check_mouse_button(1))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			helm =false;
			buyMenu = false;
		}
		break;
	case SHOP_CHEST:
		chest = true;
		buyMenu = true;
		if(IManager->check_mouse_button(1))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			chest = false;
			buyMenu = false;
		}
		break;
	case SHOP_ACCESORY:
		accesory = true;
		buyMenu = true;
		if(IManager->check_mouse_button(1))
		{
			shop_state = SHOP_SELECTION;
			main = true;
			accesory = false;
			buyMenu = false;
		}
		break;
	case SHOP_POTIONS:
		potions = true;
		buyMenu = true;
		if(IManager->check_mouse_button(1))
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
					player->getInventory();
					main = false;
					break;
				case 2: // Exit
					main = false;
					buyMenu = false;
					shop_state = SHOP_SELECTION;
					game_state = OVERWORLD;
					SManager->stopStream();
					break;
				default:
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
				default:
					break;
				}
			}
		}
	}
	if(weapons)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_WEAPONS_BUTTONS_POTITIONS; i++)
		{
			if(weapons_buttons_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				weapons_buttons_pos[i].setHighlight(true);
				weapons_buttons_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				weapons_buttons_pos[i].setHighlight(false);
				weapons_buttons_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//Check for cursor over menu option and left mouse button click
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(weapons_buttons_pos[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9,true);
						//Switch states accordingly
						switch(i)
						{
						case 0: //dagger
							if(player->getMoney() >= 100)
							{
								player->addToInventory(2);
								player->adjustMoney(-100);
							}else
								noMoney = true;
							break;
						case 1: // broad sword
							if(player->getMoney() >= 200)
							{
								player->addToInventory(2);
								player->adjustMoney(-200);
							}else
								noMoney = true;
							break;
						case 2: // rod
							if(player->getMoney() >= 200)
							{
								player->addToInventory(2);
								player->adjustMoney(-200);
							}else
								noMoney = true;
							break;
						case 3: // oak staff
							if(player->getMoney() >= 120)
							{
								player->addToInventory(2);
								player->adjustMoney(-120);
							}else
								noMoney = true;
							break;
						case 4:
							break;
						default:
							break;
						}
					}
				}
			}else IManager->set_button(DIK_9,false);
		}
	}
	if(helm)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_HELM__BUTTONS_POSITIONS; i++)
		{
			if(helm_button_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				helm_button_pos[i].setHighlight(true);
				helm_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				helm_button_pos[i].setHighlight(false);
				helm_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//Check for cursor over menu option and left mouse button click
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(helm_button_pos[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9,true);
						//Switch states accordingly
						switch(i)
						{
						case 0: // Leather Hat
							if(player->getMoney() >= 150)
							{
								player->addToInventory(0);
								player->adjustMoney(-150);
							}else
								noMoney = true;
							break;
						case 1: //
							break;
						case 2: //
							break;
						case 3: //
							break;
						case 4:
							break;
						default:
							break;
						}
					}
				}
			}else IManager->set_button(DIK_9,false);
		}
	}
	if(chest)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_HELM__BUTTONS_POSITIONS; i++)
		{
			if(chest_button_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				chest_button_pos[i].setHighlight(true);
				chest_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				chest_button_pos[i].setHighlight(false);
				chest_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//Check for cursor over menu option and left mouse button click
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(chest_button_pos[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9,true);
						//Switch states accordingly
						switch(i)
						{
						case 0: // Cloth armor
							if(player->getMoney() >= 200)
							{
								player->addToInventory(1);
								player->adjustMoney(-150);
							}else
								noMoney = true;
							break;
						case 1: //
							break;
						case 2: //
							break;
						case 3: //
							break;
						case 4:
							break;
						default:
							break;
						}
					}
				}
			}else IManager->set_button(DIK_9,false);
		}
	}
	if(accesory)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_ACCESORY_BUTTONS_POSITIONS; i++)
		{
			if(accesory_button_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				accesory_button_pos[i].setHighlight(true);
				accesory_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				accesory_button_pos[i].setHighlight(false);
				accesory_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//Check for cursor over menu option and left mouse button click
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(accesory_button_pos[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9,true);
						//Switch states accordingly
						switch(i)
						{
						case 0: //
							break;
						case 1: //
							break;
						case 2: //
							break;
						case 3: //
							break;
						case 4:
							break;
						default:
							break;
						}
					}
				}
			}else IManager->set_button(DIK_9,false);
		}
	}
	if(potions)
	{
		//Checks to see if mini buttons were pressed to switch from weapons/helms/chests/accesories/potions
		for(int i = 0; i < MAX_POTIONS_BUTTONS_POSITION; i++)
		{
			if(potion_button_pos[i].isOn(cursorPos.x, cursorPos.y, 3))
			{
				potion_button_pos[i].setHighlight(true);
				potion_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 50));
			}else
			{
				potion_button_pos[i].setHighlight(false);
				potion_button_pos[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			//Check for cursor over menu option and left mouse button click
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(potion_button_pos[i].isHighlighted())
				{
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9,true);
						//Switch states accordingly
						switch(i)
						{
						case 0: //Phoenix Down
							if(player->getMoney() >= 300)
							{
								player->addToInventory(7);
								player->adjustMoney(-300);
							}else
								noMoney = true;
							break;
						case 1: // Ether
							if(player->getMoney() >= 50)
							{
								player->addToInventory(5);
								player->adjustMoney(-50);
							}else
								noMoney = true;
							break;
						case 2: // Hi Ether
							if(player->getMoney() >= 100)
							{
								player->addToInventory(6);
								player->adjustMoney(-100);
							}else
								noMoney = true;
							break;
						case 3: // Potion
							if(player->getMoney() >= 50)
							{
								player->addToInventory(3);
								player->adjustMoney(-50);
							}else
								noMoney = true;
							break;
						case 4: // Hi Potion
							if(player->getMoney() >= 300)
							{
								player->addToInventory(4);
								player->adjustMoney(-100);
							}else
								noMoney = true;
							break;
						default:
							break;
						}
					}
				}
			}else IManager->set_button(DIK_9,false);
		}
	}
}

void Town::render(GraphicsManager2D *GManager, ID3DXSprite *spriteObj, float dt, Player *player, HWND *m_hWnd, ID3DXFont *D3DFont)
{
	//Shop Selection
	if(main)
	{
		GManager->Draw2DObject(D3DXVECTOR3(0.4f, 0.55f, 0.4f),
			D3DXVECTOR3(240.0f, 250.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			spriteObj, GRAPHICS_SHOP_KEEP, 
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
	}
}

void Town::renderText(ID3DXFont *D3DFont, Player *player, HWND *m_hWnd, float dt)
{
	RECT money;
	money.top = 500;
	money.left = 580;
	wchar_t bufferFont[64];
	swprintf_s(bufferFont, 64, L"%d", player->getMoney());
	D3DFont->DrawText(0, bufferFont, -1, &money, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(200, 255, 255, 255));

	if(noMoney)
	{
		RECT noMonBox;
		wchar_t noMonbuffer[64];
		noMonBox.left = 350;
		noMonBox.top = 550;
		swprintf_s(noMonbuffer, 64,L"You do not have enough gil!");
		D3DFont->DrawText(0, noMonbuffer, -1, &noMonBox, DT_TOP| DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 0, 0));
		count += dt;
		if(count >= 2.0f)
			noMoney = false;
	}else
		count = 0;
}