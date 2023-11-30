#include "FireFlower.h"
#include "Mario.h"
#include "Playscene.h"
#include <algorithm>

void CFireFlower::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CMario*>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}


void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CFireFlower::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CFireFlower::SetState(int state)
{
	
}

void CFireFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIRE_FLOWER_BBOX_WIDTH;
	b = y + FIRE_FLOWER_BBOX_HEIGHT;
}