#include "TextManager.h"

#define SAFE_RELEASE(x) if(x){ x->Release(); x = 0; }

TextManager* TextManager::instance(){
	static TextManager text_manager;
	return &text_manager;
};

TextManager::~TextManager(){
	SAFE_RELEASE(m_pD3DFont);
};

void TextManager::init(IDirect3DDevice9* Device){
	//  INFO:  Sets the rect for the scrolling text in the Intro game state and loads Old English Font
	counter = 0;
	rect.top = 500;
	rect.bottom = 600;
	rect.left = 50;
	rect.right = 500;
	D3DXCreateFont(Device, 30, 10, FW_BOLD, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Old English Text MT"), &m_pD3DFont);
};

void TextManager::update(float dt){
	//  INFO:  If the text has not yet reached the "top" of the screen (100 in this case), scroll the text upwards based off of delta time.
	if(rect.top > 100){
		counter += dt;
		if(counter > 0.05f){
			counter = 0.0f;
			rect.top -= 2;
		}
	}
};

void TextManager::render(){
	//  INFO:  Renders the main story.  Consider this a place holder story.
	wchar_t buffer[1024];


	swprintf_s(buffer, 1024, L"The people of Baron once lived quiet, peaceful lives.  That all changed\none fateful day.  Over twenty years ago, a vicious dragon attacked the\nvillage and killed many of it's people;  including the parents of a young\nboy named Cecil.  Then, as fast as the Dragon had come, it was gone,\nleaving a trail of devastation in it's wake.  Filled with rage, Cecil\nmvowed to one day slay the dragon and avenge his parents.  As the years\nwent by, Cecil trained to exhaustion every day in order to prepare to\nface the dragon.  ");
	m_pD3DFont->DrawText(0, buffer, -1, &rect, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

};

void TextManager::renderSavedGameText(IOManager* file, int gameState){
	RECT rectangle;
	rectangle.top = 190;
	rectangle.bottom = 290;
	rectangle.left = 200;
	rectangle.right = 400;
	wchar_t buffer[128];
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; j++){
			switch(file->m_gameSave[i][j*4]){
			case WARRIOR:
				swprintf_s(buffer, 128, L"Warrior:  %d", file->m_gameSave[i][1+j*4]);
				m_pD3DFont->DrawText(0, buffer, -1, &rectangle, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125,0));
				rectangle.top		+=	30;
				rectangle.bottom	+=	30;
				break;
			case ARCHER:
				swprintf_s(buffer, 128, L"Archer:  %d", file->m_gameSave[i][2+j*4]);
				m_pD3DFont->DrawText(0, buffer, -1, &rectangle, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125,0));
				rectangle.top		+=	30;
				rectangle.bottom	+=	30;
				break;
			case GREYMAGE:
				swprintf_s(buffer, 128, L"GreyMage:  %d", file->m_gameSave[i][3+j*4]);
				m_pD3DFont->DrawText(0, buffer, -1, &rectangle, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 125,0));
				rectangle.top		+=	30;
				rectangle.bottom	+=	30;
				break;
			}
		}
		rectangle.top += 70;
		rectangle.bottom += 70;
	}

	wchar_t currentState[16];
	RECT rect2;
	rect2.top = 70;
	rect2.left = 350;
	rect2.right = 450;
	rect2.bottom = 170;
	ZeroMemory(currentState, sizeof(currentState));
	if(gameState == SAVE){
		swprintf_s(currentState, 16, L"Save Game");
	}
	else if(gameState == LOAD)
		swprintf_s(currentState, 16, L"Load Game");
	m_pD3DFont->DrawText(0, currentState, -1, &rect2, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255,255));
};