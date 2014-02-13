#include "Cursor.h"

Cursor::Cursor()
{
	//cursorTexture = 0;
}

Cursor::~Cursor()
{}

Cursor* Cursor::instance()
{
	static Cursor cursor;
	return &cursor;
}

void Cursor::init()
{
	cursorPos.x = 500;
	cursorPos.y = 300;

	scale = D3DXVECTOR3(0.1f, 0.1f, 0.0f);
	trans = D3DXVECTOR3(cursorPos.x, cursorPos.y, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


}

void Cursor::update(float x, float y)
{
	cursorPos.x += x;
	cursorPos.y += y;

	if(cursorPos.x < 0) cursorPos.x = 0;
	if(cursorPos.y < 0) cursorPos.y = 0;
	if(cursorPos.x > 800) cursorPos.x = 800;
	if(cursorPos.y > 600) cursorPos.y = 600;

	trans.x = cursorPos.x;
	trans.y = cursorPos.y;
}

void Cursor::render(GraphicsManager2D *GManager, ID3DXSprite *SpriteObj)
{
	
	GManager->Draw2DObject(scale, trans, rot, SpriteObj, GRAPHICS_CURSOR , D3DCOLOR_ARGB(255, 255, 255, 255));
}
