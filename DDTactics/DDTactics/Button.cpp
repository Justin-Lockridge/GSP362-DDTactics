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
}
