#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class Button
{
private:
	bool highlight;
	D3DXVECTOR3 pos;
	RECT rect;
	
	D3DCOLOR color;
	//char* id;
public:
	Button();
	~Button();

	int height, width;

	void setHighlight(bool);
	bool isHighlighted();
	D3DXVECTOR3 getPos();
	void setPos(float x, float y);
	void setRect(RECT);
	RECT getRect(){return rect;}

	void setColor(D3DCOLOR a_color);
	D3DCOLOR getColor();

	bool isOn(float x, float y, int scale);
};
