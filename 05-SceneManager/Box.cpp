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

// BOX (Merge box)
void CBoxs::PushBox(CBox* box)
{
	listBox.push_back(box);
}

void CBoxs::Render()
{
	for (size_t i = 0; i < listBox.size(); i++)
	{
		listBox[i]->Render();
	}
}

void CBoxs::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y;
}
void CBoxs::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

