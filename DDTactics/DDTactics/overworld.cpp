///////////////////////////////////////////////////////////////////////
// Overworld map
//	including shops and transitions into status window and battle
///////////////////////////////////////////////////////////////////////
#include "Overworld.h"


Overworld::Overworld()
{
	overworld_state = OVERWORLD_SELECTION;
	m_Overworld_Texture = 0;
	m_Char_texture = 0;
	m_Area_texture = 0;
	m_menu_background = 0;
	m_menu_words = 0;

	m_OverworldMusic = 0;
	m_TownMusic = 0;

}

Overworld::~Overworld()
{

}


void Overworld::init(IDirect3DDevice9 *device, FMOD::System *fmodSystem)
{
	Area_location location[] = 
	{
		{ 300, 250 },
		{ 0, 0 },
		{ 0, 0 },

	};

	RectData temp[] =
	{

		{0, 90, 90,140 },
		{0, 90, 140, 190},
		{0, 90, 190, 250},


	};

	for(int i = 0; i < MAX_AREAS; i++)
	{
		map[i].node_position.x = location[i].x;
		map[i].node_position.y = location[i].y;
	}

	for(int i = 0; i < 3; i++)
	{
		char_sprite_position[i].bottom = temp[i].b;
		char_sprite_position[i].left = temp[i].l;
		char_sprite_position[i].right = temp[i].r;
		char_sprite_position[i].top = temp[i].t;
	}

	//Simple node links for now
	//TODO: Create a struct containing data on which areas to link to which direction
	map[0].down = &map[1];
	map[1].up = &map[0];
	map[1].left = &map[2];
	map[2].right = &map[1];

	//re-init overworld-state in case of loading a game
	overworld_state = OVERWORLD_SELECTION;
	//town_state = TOWN_SELECT;

	//init player starting position at 0
	overworld_player.pos.x = map[0].node_position.x;
	overworld_player.pos.y = map[0].node_position.y;
	overworld_player.current_node_position = &map[0];

	screen_pos.x = -400;  //left 640, right -600  center 20
	screen_pos.y = 360;  //top 360, bottom -50    center 155

	count = 0;
	count2 = 0;

	menu_position.x = 700.0f;
	menu_position.y = 100.0f;
	menu_alpha = 128; 

	D3DXCreateTextureFromFileEx(device, L"TOmap.jpg", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(0,255,255,255), &m_Overworld_Info, 0, &m_Overworld_Texture);


	D3DXCreateTextureFromFileEx(device, L"DK_Cecil.png", 0,0,0,0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(0,255,255,255), &m_O_Char_Info, 0, &m_Char_texture);

	D3DXCreateTextureFromFileEx(device, L"O_Menu_background.png", 0,0,0,0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(0,255,255,255), &m_menu_background_info, 0, &m_menu_background);

	D3DXCreateTextureFromFileEx(device, L"O_Menu_words.png", 0,0,0,0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(0,255,255,255), &m_words_info, 0, &m_menu_words);
		


	fmodSystem->createStream("AbSolitude.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &m_TownMusic);
	fmodSystem->createStream("RO Juno.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &m_OverworldMusic);
}


void Overworld::render( ID3DXSprite *drawSpriteObj, 
					   IDirect3DTexture9 *cursor, D3DXIMAGE_INFO &cursor_info, D3DVECTOR &cursor_pos, float dt)
{
	if(SUCCEEDED(drawSpriteObj->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK)))
	{

		D3DXMATRIX rotMat, scaleMat, transMat, worldMat;

		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&worldMat);

		switch(overworld_state)
		{
		case OVERWORLD_TOWN:
			break;
		case OVERWORLD_SHOP:
			break;
		default:

			//Draw the map
			D3DXMatrixScaling(&scaleMat, 0.5f, 0.5f, 1.0f);
			D3DXMatrixTranslation(&transMat, screen_pos.x, screen_pos.y, 0.0f);
			//D3DXMatrixRotationY(&rotMat, D3DXToRadian(90.0f));
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			drawSpriteObj->SetTransform(&worldMat);

			drawSpriteObj->Draw(m_Overworld_Texture, 0, &D3DXVECTOR3(m_Overworld_Info.Width *0.5f, m_Overworld_Info.Height * 0.5f, 0),
				0, D3DCOLOR_ARGB(255,255,255,255));

			//Draw the path and nodes


			//Draw the player


			count++;
			if( count == 240)
			{
				count = 0;
				count2++;
				if(count2 >= 3)
					count2 = 0;
			}
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat, 1.0f, 1.0f, 1.0f);
			D3DXMatrixTranslation(&transMat, overworld_player.pos.x,  overworld_player.pos.y, 0.0f);
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			drawSpriteObj->SetTransform(&worldMat);
			drawSpriteObj->Draw(m_Char_texture, &char_sprite_position[count2], 
				&D3DXVECTOR3((char_sprite_position[count2].right - char_sprite_position[count2].left) *0.5f,
				(char_sprite_position[count2].bottom - char_sprite_position[count2].top) * 0.5f,
				0.0f), 0, D3DCOLOR_ARGB(255, 255, 255, 255));


			//Draw the menu
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat, 0.5f, 0.5f, 0.0f);
			D3DXMatrixTranslation(&transMat, menu_position.x, menu_position.y, 0.0f);
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			drawSpriteObj->SetTransform(&worldMat);
			drawSpriteObj->Draw(m_menu_background, 0, &D3DXVECTOR3(m_menu_background_info.Width * 0.5f, 
																	m_menu_background_info.Height * 0.5f, 0.0f), 
								0, D3DCOLOR_ARGB(menu_alpha,255,255,255));

			//Draw the font
			drawSpriteObj->Draw(m_menu_words, 0, &D3DXVECTOR3(m_words_info.Width *0.5f,
																m_words_info.Height *0.5f, 0.0f),
								0, D3DCOLOR_ARGB(255,255,255,255));


			//Draw the cursor
			D3DXMatrixIdentity(&rotMat);
			D3DXMatrixIdentity(&scaleMat);
			D3DXMatrixIdentity(&transMat);
			D3DXMatrixIdentity(&worldMat);

			D3DXMatrixScaling(&scaleMat, 0.1f, 0.1f, 0.0f);
			D3DXMatrixTranslation(&transMat, cursor_pos.x, cursor_pos.y, 0.0f);
			D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);
			D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);

			drawSpriteObj->SetTransform(&worldMat);
			drawSpriteObj->Draw(cursor, 0, &D3DXVECTOR3(cursor_info.Width *0.5f, cursor_info.Height *0.5f, 0.0f), 0,
				D3DCOLOR_ARGB(255, 255, 255, 255));

		}
		

		drawSpriteObj->End();


	}
}


void Overworld::update(int GAMESTATEBATTLE, int &game_state, DIMOUSESTATE2 &a_mouseState, 
					   char buffer[255], FMOD::System* fmodSystem, FMOD::Channel* m_musicChannel, 
					   D3DVECTOR &cursor_pos)
{


	switch(overworld_state)
	{
	case OVERWORLD_SELECTION:
		cursor_pos.x += a_mouseState.lX;
		cursor_pos.y += a_mouseState.lY;

		if(cursor_pos.x <0)		cursor_pos.x=0;
		if(cursor_pos.y <0)		cursor_pos.y=0;
		if(cursor_pos.x >800)		cursor_pos.x=800;
		if(cursor_pos.y >600)		cursor_pos.y=600;

		if(cursor_pos.x >= 790)
		{
			screen_pos.x -= 2;
			overworld_player.pos.x -= 2;
			if(screen_pos.x < -600)
			{
				screen_pos.x = -600;
				overworld_player.pos.x += 2;
			}				
		}

		if(cursor_pos.x <= 10)
		{
			screen_pos.x += 2;
			overworld_player.pos.x += 2;
			if(screen_pos.x > 640)
			{
				screen_pos.x = 640;
				overworld_player.pos.x -= 2;				
			}
		
		}

		if(cursor_pos.y >= 590)
		{
			screen_pos.y -= 2;
			overworld_player.pos.y -= 2;
			if(screen_pos.y < -50)
			{
				screen_pos.y = -50;
				overworld_player.pos.y += 2;
			}
		
		}

		if(cursor_pos.y <= 10)
		{
			screen_pos.y += 2;
			overworld_player.pos.y += 2;
			if(screen_pos.y > 360)
			{
				screen_pos.y = 360;
				overworld_player.pos.y -= 2;				
			}
		
		}
		break;
	case OVERWORLD_TRANSITION:
		break;
	case OVERWORLD_TOWN:

		switch(town_state)
		{
		case TOWN_SELECT:
			break;
		case TOWN_SHOP:
			overworld_state = OVERWORLD_SHOP;
			break;
		case TOWN_RECRUIT:
			break;
		case TOWN_EXIT:
			overworld_state = OVERWORLD_SELECTION;
			break;		
		}
		break;
	case OVERWORLD_SHOP:
		break;
	case OVERWORLD_MENU:
		break;
	case OVERWORLD_STATUS:
		break;
	case OVERWORLD_BATTLE:
		overworld_state = OVERWORLD_SELECTION;
		game_state = GAMESTATEBATTLE;
		break;
	case OVERWORLD_SAVE:
		break;
	case OVERWORLD_LOAD:
		break;



	}



}


void Overworld::shutdown()
{
	if(m_menu_background)
	{
		m_menu_background->Release();
		m_menu_background = 0;
	}

	if(m_menu_words)
	{
		m_menu_words->Release();
		m_menu_words = 0;
	}

	if(m_Overworld_Texture)
	{
		m_Overworld_Texture->Release();
		m_Overworld_Texture = 0;
	}

	if(m_Char_texture)
	{
		m_Char_texture->Release();
		m_Char_texture = 0;
	}

	if(m_Area_texture)
	{
		m_Area_texture->Release();
		m_Area_texture = 0;
	}

	if(m_OverworldMusic)
	{
		m_OverworldMusic->release();
		m_OverworldMusic = 0;
	}

	if(m_TownMusic)
	{
		m_TownMusic->release();
		m_TownMusic = 0;
	}

}







