#pragma once

#include "InputManager.h"
#include "GraphicsManager2D.h"
#include "GameEnums.h"

class Cursor
{
private:

	D3DXVECTOR3	scale, trans, rot;

	Cursor();

public:
	~Cursor();
	static Cursor* instance();

	D3DXVECTOR2		cursorPos;

	void init();
	void update(float x, float y);
	void render(GraphicsManager2D *GManager, ID3DXSprite *SpriteObj);

	
};