#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "IOManager.h"

class TextManager
{
private:
	ID3DXFont	*m_pD3DFont;	//Font Object
	RECT		rect;
	float		counter;
public:
	static TextManager* instance();
	~TextManager();

	void init(IDirect3DDevice9* Device);
	
	//  INFO:  Update function for scrolling text
	void update(float dt);

	void render();
	void renderSavedGameText(IOManager* file, int gameState);


};