#include "StatusMenu.h"
//#include "MiscFunctions.cpp"

StatusMenu::StatusMenu()
{}

StatusMenu::~StatusMenu()
{}

StatusMenu* StatusMenu::instance()
{
	static StatusMenu status_menu;
	return &status_menu;
}


void StatusMenu::init(Player *player)
{
	status_state = STATUS_MAIN;

	//initialize buttons
	RData temp_button[] = 
	{
		{15, 0, 45, 70, 725, 50, false },
		{90, 0, 120, 70, 725, 100, false},
		{165, 0, 195, 70, 730, 170, false},
		{240, 0, 270, 70, 725, 230, false},
		{315,0, 345, 75, 725, 290, false},
		{390, 0, 420, 70, 725, 360, false},
		{465, 0, 495, 70, 725, 440, false},
	};

	PushDemButtons(temp_button, MAX_STATUS_BUTTONS, status_menu_buttons);

	RData tempItem_Buttons[] =
	{
		{95, 0, 140, 120, 125, 70, false },
		{220, 0,265, 120, 385, 70, false },
		{345, 0, 390, 120, 625, 70, false},
	
	};

	PushDemButtons(tempItem_Buttons, 3, status_item_buttons);
	
	
	RData tempArrow_Button_Main[] =
	{
		{0,0,150,125, 320, 40, false},
		{0,0,150,125, 320, 465, false},
	};

	PushDemButtons(tempArrow_Button_Main, 2, status_menu_arrow_button);

	screenPos.push_back(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	screenPos.push_back(D3DXVECTOR3(100.0f, 250.0f, 0.0f));
	screenPos.push_back(D3DXVECTOR3(100.0f, 400.0f, 0.0f));

	//Save current player characters for future checks in the event the player recruits more characters
	playerCharacters = player->returnArmy()->size();

	//Create a circular linked list

	for(unsigned int i = 0; i < player->returnArmy()->size(); i++)
	{
		//set next character
		if((i+1) == player->returnArmy()->size())  //if we reached the last character, set next to the first
			player->getCharacter(i)->setNext(player->getCharacter(0));
		else
			player->getCharacter(i)->setNext(player->getCharacter(i+1));


		//set previous character
		if(  i == 0)  //if we reached the first character, set previous to the last
			player->getCharacter(i)->setPrevious(player->getCharacter(player->returnArmy()->size() - 1));
		else
			player->getCharacter(i)->setPrevious(player->getCharacter(i - 1));
	}

	//Grab only 3 characters to draw to start off with
	if(player->returnArmy()->size() >= 3)
	{
		drawChar.push_back(player->getCharacter(0));
		drawChar.push_back(player->getCharacter(1));
		drawChar.push_back(player->getCharacter(2));
	}else
	{
		//if there are less than 3 characters available, grab them 
		for(unsigned int i = 0; i < player->returnArmy()->size(); i++)
		{
			drawChar.push_back(player->getCharacter(i));
		}
	}

	

}

void StatusMenu::Update(Cursor *cursor, InputManager *IManager, SoundManager *SManager, Player *player, int &game_state, float dt)
{
	//update the Linked list in the event the player has added more characters
	if(player->returnArmy()->size() > playerCharacters)
		recreateList(player);

	switch(status_state)
	{
	case STATUS_MAIN:
		//Check if side button are moused over
		for(int i = 0; i < MAX_STATUS_BUTTONS; i++)
		{
			//change button highlight and color
			status_menu_buttons[i].checkOn(cursor->cursorPos.x, cursor->cursorPos.y, 2);
			//check for left mouse button press
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(status_menu_buttons[i].isHighlighted())
				{
					//Limit the check by forcing a check on another button
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

		//check if arrow buttons are moused over
		for(int i = 0; i < status_menu_arrow_button.size(); i++)
		{
			status_menu_arrow_button[i].checkOn(cursor->cursorPos.x, cursor->cursorPos.y, 4);

			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON))
			{
				if(status_menu_arrow_button[i].isHighlighted())
				{
					//Limit the check by forcing a check on another button
					if(!IManager->check_button_down(DIK_9))
					{
						IManager->set_button(DIK_9, true);

						//change characters depending on which arrow was pushed
						switch(i)
						{
						case 0:
							//change the character drawn
							
							//Check the size of the player's army, if it's less than 3, do nothing

							if(player->returnArmy()->size() < 3)
								break;
							
							//Top character is pulled out, middle character goes to the top,
							//bottom character goes to the middle, the next character is added at the bottom
							//if there is no new character, the old top character becomes the bottom
							for(unsigned int i = 0; i < drawChar.size(); i++)
							{
								//Copy next characters into the copy container
								copyChar.push_back(drawChar[i]->getNext());
							}

							//clear drawChar
							drawChar.clear();
							//push copyChar characters to drawChar and clear copy
							for(unsigned int i = 0; i < copyChar.size(); i++)
							{
								drawChar.push_back(copyChar[i]);
							}

							//clear copyChar for reuse
							copyChar.clear();
							

							break;
						case 1:
							//Check the size of the player's army, if it's less than 3, do nothing

							if(player->returnArmy()->size() < 3)
								break;

							//Bottom character is pulled out, middle character goes to the bottom,
							//Top character goes to the middle, the previous character is added to the top
							//if there is no new character, the old bottom character becomes the top
							for(unsigned int i = 0; i < drawChar.size(); i++)
							{
								//Copy next characters into the copy container
								copyChar.push_back(drawChar[i]->getPrevious());
							}

							//clear drawChar
							drawChar.clear();
							//push copyChar characters to drawChar and clear copy
							for(unsigned int i = 0; i < copyChar.size(); i++)
							{
								drawChar.push_back(copyChar[i]);
							}

							//clear copyChar for reuse
							copyChar.clear();
						
							break;
						}

					}
				}
			}else
				IManager->set_button(DIK_9, false);

		}
		
		break;
	case STATUS_ITEM:
		for(unsigned int i = 0; i < status_item_buttons.size(); i++)
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

		
		
		
		
		break;
	case STATUS_ABILITY:
		//TODO:  Similar to item, USE, ORGANIZE, BACK
		//USE has no use, but shows description

		//ORGANIZE currently has no function

		//BACK goes back to STATUS_MAIN
		break;
	case STATUS_MAGIC:
		//TODO:  Similar to item, USE, ORGANIZE, BACK


		break;
	case STATUS_JOB:
		//Starts on the first character, arrows left and right to click and cycle through.
		//Show current job
		//Show list of jobs
		//Click on different job changes character job

		break;
	case STATUS_STATUS:
		//Starts on the first character, arrows left and right to click and cycle through
		//Display character status and equip
		//Change equipment throught the status screen
		break;
	case STATUS_ORDER:
		//Not implemented yet
		status_state = STATUS_MAIN;
		break;
	case STATUS_BACK:
		game_state = OVERWORLD;
		status_state = STATUS_MAIN;
		break;
	case STATUS_ITEM_USE:

		if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON))
			status_state = STATUS_ITEM;
		//NOTE:  Use currently has no use aside from being able to highlight the items and display item description

		//TODO:  Draw Text for each item in inventory and how many there are

		//TODO:  If highlighted, the item will display its description at the bottom

		//TODO:  Draw Item Description at the bottom
		break;
	case STATUS_ITEM_ORGANIZE:
		//NOTE: Organize currently has no use, return state to STATUS_ITEM
		status_state = STATUS_ITEM;
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

		for(unsigned int i = 0; i < status_item_buttons.size(); i++)
		{
			GManager->DrawButton( D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(status_item_buttons[i].getPos().x, status_item_buttons[i].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				status_item_buttons[i].getRect(), spriteObj, GRAPHICS_ITEM_BUTTONS,
				status_item_buttons[i].width, status_item_buttons[i].height, status_item_buttons[i].getColor());

		
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
	case STATUS_ITEM_USE:
		GManager->Draw2DObject(D3DXVECTOR3(0.775f, 0.575f, 1.0f),
							   D3DXVECTOR3(312.0f, 180.f, 0.0f),
							   D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							   spriteObj,
							   GRAPHICS_ITEM_BACKGROUND,
							   D3DCOLOR_ARGB(255,255,255,255)
							   );

		for(unsigned int i = 0; i < status_item_buttons.size(); i++)
		{
			GManager->DrawButton( D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(status_item_buttons[i].getPos().x, status_item_buttons[i].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				status_item_buttons[i].getRect(), spriteObj, GRAPHICS_ITEM_BUTTONS,
				status_item_buttons[i].width, status_item_buttons[i].height, status_item_buttons[i].getColor());

		
		}
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

		//TODO:  Draw characters, be able to click on arrows to cycle through characters
		
			GManager->DrawButton( D3DXVECTOR3(0.5f, 0.5f, 1.0f),
				D3DXVECTOR3(status_menu_arrow_button[0].getPos().x, status_menu_arrow_button[0].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 270.0f),
				status_menu_arrow_button[0].getRect(), spriteObj, GRAPHICS_ITEM_ARROW,
				status_menu_arrow_button[0].width, status_menu_arrow_button[0].height,
				status_menu_arrow_button[0].getColor()
				);

			GManager->DrawButton( D3DXVECTOR3(0.5f, 0.5f, 1.0f),
				D3DXVECTOR3(status_menu_arrow_button[1].getPos().x, status_menu_arrow_button[1].getPos().y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 90.0f),
				status_menu_arrow_button[1].getRect(), spriteObj, GRAPHICS_ITEM_ARROW,
				status_menu_arrow_button[1].width, status_menu_arrow_button[1].height,
				status_menu_arrow_button[1].getColor()
				);
			
			
									

			for(int i = 0; i < drawChar.size(); i++)
			{
				GManager->Draw2DObject(D3DXVECTOR3(0.5f, 0.7f, 1.0f),
										screenPos[i],
										D3DXVECTOR3(0.0f, 0.0f,0.0f),
										spriteObj,
										GRAPHICS_BSTATS_WINDOW,
										D3DCOLOR_ARGB(255,255,255,255));


				switch(drawChar[i]->getCurrentJob())
				{
				case WARRIOR:
					GManager->Draw2DObject(D3DXVECTOR3(0.16f, 0.15f, 1.0f),
											screenPos[i], D3DXVECTOR3(0.0f,0.0f,0.0f),
											spriteObj, GRAPHICS_WAR_ICON, D3DCOLOR_ARGB(255,255,255,255));
					break;
				case ARCHER:
					GManager->Draw2DObject(D3DXVECTOR3(0.16f, 0.15f, 1.0f),
											screenPos[i], D3DXVECTOR3(0.0f,0.0f,0.0f),
											spriteObj, GRAPHICS_ARCHER_ICON, D3DCOLOR_ARGB(255,255,255,255));
					break;
				case GREYMAGE:
					GManager->Draw2DObject(D3DXVECTOR3(0.16f, 0.15f, 1.0f),
											screenPos[i], D3DXVECTOR3(0.0f,0.0f,0.0f),
											spriteObj, GRAPHICS_MAGE_ICON, D3DCOLOR_ARGB(255,255,255,255));
					break;
				
				}
			
			}
	
			
		break;

	}

}

void StatusMenu::drawText(ID3DXFont *font, Player *player)
{
	switch(status_state)
	{
	case STATUS_ITEM:
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
		break;
	default:
		RECT r;
		r.left = 680;
		r.top = 525;
		swprintf_s(word, 64, L"Money\n  %d", player->getMoney());
		font->DrawText(0, word, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255,255,255,255));

		r.left = 260;
		r.top = 65;
		for(unsigned int i = 0; i < drawChar.size(); i++)
		{
			swprintf_s(word, 64, drawChar[i]->getName());
			font->DrawText(0, word, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255,255,255,255));
			r.top += 150;
		
		}
		
		r.left = 280;
		r.top = 90;
		for(unsigned int i = 0; i < drawChar.size(); i++)
		{
			switch(drawChar[i]->getCurrentJob())
			{
			case WARRIOR:
				swprintf_s(word, 64, L"Warrior");
				break;
			case ARCHER:
				swprintf_s(word, 64, L"Archer");
				break;
			case GREYMAGE:
				swprintf_s(word, 64, L"Mage");
				break;			
			
			}
			font->DrawText(0, word, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255,255,255,255));
			r.top += 150;

		}

		r.left = 240;
		r.top = 115;
		for(unsigned int i = 0; i < drawChar.size(); i++)
		{
			swprintf_s(word, 64, L"HP %d", drawChar[i]->getCharacterStats().max_health);
			font->DrawText(0, word, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255,255,255,255));
			r.left += 90;
			swprintf_s(word, 64, L"MP %d", drawChar[i]->getCharacterStats().max_mana);
			font->DrawText(0, word, -1, &r, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255,255,255,255));
			r.left = 240;
			r.top += 150;
		}

	


		break;
	
	
	
	
	}
}

void StatusMenu::recreateList(Player* player)
{
	//Recreate the entire list in the event that the possibility of a character in the middle is removed
	//or if a character is added in the middle instead of the end.
	
	//Save new current player characters for future checks in the event the player recruits more characters
	//or removes characters
	playerCharacters = player->returnArmy()->size();

	//Create a circular linked list

	for(unsigned int i = 0; i < player->returnArmy()->size(); i++)
	{
		//set next character
		if((i+1) == player->returnArmy()->size())  //if we reached the last character, set next to the first
			player->getCharacter(i)->setNext(player->getCharacter(0));
		else
			player->getCharacter(i)->setNext(player->getCharacter(i+1));


		//set previous character
		if(i == 0)  //if we reached the first character, set previous to the last
			player->getCharacter(i)->setPrevious(player->getCharacter(player->returnArmy()->size() - 1));
		else
			player->getCharacter(i)->setPrevious(player->getCharacter(i - 1));
	}

	//Grab only 3 characters to draw to start off with
	if(player->returnArmy()->size() >= 3)
	{
		drawChar.push_back(player->getCharacter(0));
		drawChar.push_back(player->getCharacter(1));
		drawChar.push_back(player->getCharacter(2));
	}else
	{
		//if there are less than 3 characters available, grab them 
		for(unsigned int i = 0; i < player->returnArmy()->size(); i++)
		{
			drawChar.push_back(player->getCharacter(i));
		}
	}

}

void StatusMenu::PushDemButtons(RData buttons[], int size, std::vector<Button>& container)
{
	for(int i = 0; i < size; i++)
	{
		Button temp;
		RECT r;

		r.top = buttons[i].t;
		r.left = buttons[i].l;
		r.right = buttons[i].r;
		r.bottom = buttons[i].b;

		temp.setRect(r);
		temp.setPos(buttons[i].x, buttons[i].y);
		temp.setHighlight(buttons[i].highlight);
		temp.setColor(D3DCOLOR_XRGB(255,255,255));

		container.push_back(temp);
		
	}
}

void StatusMenu::shutdown()
{
	//clear vectors
	drawChar.clear();

	copyChar.clear();

}