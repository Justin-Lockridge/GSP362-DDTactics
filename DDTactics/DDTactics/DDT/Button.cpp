#include "Button.h"


Button::Button()
{
	highlight = false;
	pos.x=0;
	pos.y=0;
	pos.z=0;
}
Button::~Button()
{
}

void Button::setHighlight(bool a_bool)
{
	highlight = a_bool;
}
bool Button::isHighlighted()
{
	return highlight;
}
D3DXVECTOR3 Button::getPos()
{
	return pos;
}
void Button::setPos(float x, float y)
{
	pos.x=x;
	pos.y=y;
}

void Button::setRect(RECT a_rect)
{
	rect.bottom = a_rect.bottom;
	rect.left = a_rect.left;
	rect.right = a_rect.right;
	rect.top = a_rect.top;

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

void Button::setColor(D3DCOLOR a_color)
{
	color = a_color;
}

D3DCOLOR Button::getColor()
{
	return color;
}

bool Button::isOn(float x, float y, int scale)
{
	
	return (x < (pos.x + (width / scale)) && x > (pos.x -  (width / scale)) &&
		y < (pos.y + (height / scale)) && y > (pos.y -  (height / scale)));

	/*if( x < (pos.x + (float)(width / scale)) && x > (pos.x - (float) (width / scale)) &&
		y < (pos.y + (float) (height / scale)) && y > (pos.y - (float) (height / scale)))
		return true;
	return false;*/
}