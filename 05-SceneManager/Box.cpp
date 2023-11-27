#include "Box.h"

void CBox::Render()
{
	animation_set->at(0)->Render(x, y);
}

CBox::CBox(float status)
{
	this->status = status;
}

void CBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x+BOX_BBOX_HEIGHT;
	b = y+BOX_BBOX_WIDTH;
}

